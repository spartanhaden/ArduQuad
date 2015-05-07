
// Define the address of the MPU and it's internal Magnetometer
const int MPU_ADDR	= 104;
const int MAG_ADDR	= 12;

// Define MAG Registers
const byte MAG_WHO_AM_I		= 0x00;	// Should return 0x48
const byte MAG_INFO			= 0x01;	// Does Nothing?
const byte MAG_STATUS_1		= 0x02;	// Val of 1 = ready, 0 = normal, Data ready status bit 0
const byte MAG_XOUT_L		= 0x03;
const byte MAG_XOUT_H		= 0x04;
const byte MAG_YOUT_L		= 0x05;
const byte MAG_YOUT_H		= 0x06;
const byte MAG_ZOUT_L		= 0x07;
const byte MAG_ZOUT_H		= 0x08;
const byte MAG_STATUS_2		= 0x09;	// Val of 1 = error, 0 = normal, magnetic sensor overflow bit 3 and data read error bit 2
const byte MAG_CONTROL		= 0x0A;	// Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
const byte MAG_SELF_TEST	= 0x0C;	// Write 1 to enable self test on bit 6
const byte MAG_SENS_ADJ_X	= 0x10;	// Fuse ROM X-axis sensitivity adjustment value
const byte MAG_SENS_ADJ_Y	= 0x11;	// Fuse ROM Y-axis sensitivity adjustment value
const byte MAG_SENS_ADJ_Z	= 0x12;	// Fuse ROM Z-axis sensitivity adjustment value

// Define General Registers
const int INT_PIN_CFG	= 55;
const int USER_CTRL		= 106;
const int PWR_MGMT_1	= 107;
const int PWR_MGMT_2	= 108;
const int WHO_AM_I		= 117;
// Define ACCEL Registers
const int ACCEL_XOUT_H	= 59;
const int ACCEL_XOUT_L	= 60;
const int ACCEL_YOUT_H	= 61;
const int ACCEL_YOUT_L	= 62;
const int ACCEL_ZOUT_H	= 63;
const int ACCEL_ZOUT_L	= 64;
// Define TEMP Registers
const int TEMP_OUT_H	= 65;
const int TEMP_OUT_L	= 66;
// Define GYRO Registers
const int GYRO_XOUT_H	= 67;
const int GYRO_XOUT_L	= 68;
const int GYRO_YOUT_H	= 69;
const int GYRO_YOUT_L	= 70;
const int GYRO_ZOUT_H	= 71;
const int GYRO_ZOUT_L	= 72;


int accel[3];			// Accelerometer
int gyro[3];			// Gyroscope
int mag[3];				// Magnetometer
float magAdjVals[3];	// Magnetometer sensitivity adjustment values
const float axisMultiplier[] = {
	1.0, 1.0, 1.0
};
const int axisShift[] = {
	0, -34, -36
};
int temp;				// Raw Temperature sensor value
float tempInC;			// Temperature data in Celsius
float tempInF;			// Temperature data in Fahrenheit

boolean initSensors(){	// Initializes the Magnetometer, Gyroscope, and Accelerometer. Returns false if initialization fails
	writeByte(MPU_ADDR, PWR_MGMT_1, 128);	// PWR_MGMT_1 send 128, reset device
	delay(100);
	writeByte(MPU_ADDR, PWR_MGMT_1, 1);		// PWR_MGMT_1 send 1, set clock source to X axis Gyroscope and set sensor active
	delay(200);
	writeByte(MPU_ADDR, INT_PIN_CFG, 2);	// Enable I2C pass through
	delay(5);
	if(readByte(MPU_ADDR, WHO_AM_I) != 104)			return false;
	else if(readByte(MAG_ADDR, MAG_WHO_AM_I) != 72)	return false;
	else if(!magSelfTest())							return false;
	setMagAdjValues();
	return true;
}

void sensorLoop(){
	delay(100);
	if(readByte(MPU_ADDR,PWR_MGMT_1) > 1) initSensors();	// Checks for disconnection of Sensor
	//updateSensors();
	readAccel();
	readGyro();
	calculateAngles();
	printSensorValues();
}

void calculateAngles(){	// Calculates the angle from accelerometer data
	accelInG[0] = accel[0] / 16384.0;
	accelInG[1] = accel[1] / 16384.0;
	accelInG[2] = accel[2] / 16384.0;
	float R = sqrt(pow(accelInG[0], 2) + pow(accelInG[1], 2) + pow(accelInG[2], 2));	// Normalizes the data so the total force = 1G
	accelInG[0] = asin(accelInG[0] / R) * (180.0 / PI);
	accelInG[1] = asin(accelInG[1] / R) * (180.0 / PI);
	accelInG[2] = asin(accelInG[2] / R) * (180.0 / PI);
}

boolean magSelfTest(){	// Performs a self test on the Magnetometer, returns FALSE if the test fails
	int magSelfTest[3];
	boolean testPassed = true;
	writeByte(MAG_ADDR, MAG_CONTROL, 0);	// Set Power-down mode
	writeByte(MAG_ADDR, MAG_SELF_TEST, 64);	// Write “1” to SELF bit of self-test register
	writeByte(MAG_ADDR, MAG_CONTROL, 8);	// Set Self-test Mode
	for(int i = 0; i < 100; i++){			// Waits up to 10 ms for the Magnetometer data to become available should take 0.3 ms
		if(magDataReady()){
			break;
		}
		delayMicroseconds(100);
	}
	if(!magDataReady()){
		testPassed = false;	// Check Data Ready
		}else{
		magSelfTest[0] = readSensor(MAG_ADDR, MAG_XOUT_L, MAG_XOUT_H);				// Get X-axis self-test value
		magSelfTest[1] = readSensor(MAG_ADDR, MAG_YOUT_L, MAG_YOUT_H);				// Get Y-axis self-test value
		magSelfTest[2] = readSensor(MAG_ADDR, MAG_ZOUT_L, MAG_ZOUT_H);				// Get Z-axis self-test value
		if(magSelfTest[0] < -100 || magSelfTest[0] > 100) testPassed = false;		// Test X-axis self-test value
		else if(magSelfTest[1] < -100 || magSelfTest[1] > 100) testPassed = false;	// Test Y-axis self-test value
		else if(magSelfTest[2] < -1000 || magSelfTest[2] > -300)testPassed = false;	// Test Z-axis self-test value
	}
	writeByte(MAG_ADDR, MAG_SELF_TEST, 0);	// Write “0” to SELF bit of self-test register
	return testPassed;
}

boolean magDataReady(){	// Checks if the Magnetometer data is ready
	return readByte(MAG_ADDR, MAG_STATUS_1) == 1;
}

void setMagAdjValues(){	// Reads the factory calibration values from the Magnetometer
	short rawVals[3];
	writeByte(MAG_ADDR, MAG_CONTROL, 15);				// Sets the Magnetometer to Fuse ROM mode;
	rawVals[0] = readByte(MAG_ADDR, MAG_SENS_ADJ_X);	// Reads X-axis sensitivity adjustment value of the Magnetometer
	rawVals[1] = readByte(MAG_ADDR, MAG_SENS_ADJ_Y);	// Reads Y-axis sensitivity adjustment value of the Magnetometer
	rawVals[2] = readByte(MAG_ADDR, MAG_SENS_ADJ_Z);	// Reads Z-axis sensitivity adjustment value of the Magnetometer
	writeByte(MAG_ADDR, MAG_CONTROL, 0);				// Shuts the Magnetometer off;
	for(int i = 0; i < 3; i++){
		magAdjVals[i] = (((rawVals[i] - 128) / 256.0) + 1) * axisMultiplier[i];	// Calculates the multiplier to adjust the Magnetometer data with
	}
}

void printSensorValues(){	// Prints human readable sensor information
	Serial1.print("Accelerometer X: ");
	Serial1.print(float(accel[0])/16384);
	Serial1.print("\tY: ");
	Serial1.print(float(accel[1])/16384);
	Serial1.print("\tZ: ");
	Serial1.println(float(accel[2])/16384);
	Serial1.print("Gyroscope X: ");
	Serial1.print(gyro[0]);
	Serial1.print("\tY: ");
	Serial1.print(gyro[1]);
	Serial1.print("\tZ: ");
	Serial1.println(gyro[2]);
	Serial1.print("Magnetometer X: ");
	Serial1.print(mag[0]);
	Serial1.print("\tY: ");
	Serial1.print(mag[1]);
	Serial1.print("\tZ: ");
	Serial1.println(mag[2]);
	Serial1.print("Temperature: ");
	Serial1.print(tempInC);
	Serial1.print(" C\t");
	Serial1.print(tempInF);
	Serial1.println(" F");
	Serial1.println();
}

void readAccel(){	// Takes a reading from the Accelerometer
	accel[0] = readSensor(MPU_ADDR, ACCEL_XOUT_L, ACCEL_XOUT_H);	// Reads X-axis of Accelerometer
	accel[1] = readSensor(MPU_ADDR, ACCEL_YOUT_L, ACCEL_YOUT_H);	// Reads Y-axis of Accelerometer
	accel[2] = readSensor(MPU_ADDR, ACCEL_ZOUT_L, ACCEL_ZOUT_H);	// Reads Z-axis of Accelerometer
}

void readGyro(){	// Takes a reading from the Gyroscope
	gyro[0] = readSensor(MPU_ADDR, GYRO_XOUT_L, GYRO_XOUT_H);		// Reads X-axis of Gyroscope
	gyro[1] = readSensor(MPU_ADDR, GYRO_YOUT_L, GYRO_YOUT_H);		// Reads Y-axis of Gyroscope
	gyro[2] = readSensor(MPU_ADDR, GYRO_ZOUT_L, GYRO_ZOUT_H);		// Reads Z-axis of Gyroscope
}

void readMag(){	// Takes a reading from the Magnetometer
	writeByte(MAG_ADDR, MAG_CONTROL, 1);	// Sets the Magnetometer to single measurement mode
	for(int i = 0; i < 100; i++){			// Waits up to 10 ms for the Magnetometer data to become available should take 0.3 ms
		if(magDataReady()){
			mag[0] = (int) readSensor(MAG_ADDR, MAG_XOUT_L, MAG_XOUT_H) * magAdjVals[0] + axisShift[0];	// Reads X-axis of Magnetometer and performs corrections on it
			mag[1] = (int) readSensor(MAG_ADDR, MAG_YOUT_L, MAG_YOUT_H) * magAdjVals[1] + axisShift[1];	// Reads Y-axis of Magnetometer and performs corrections on it
			mag[2] = (int) readSensor(MAG_ADDR, MAG_ZOUT_L, MAG_ZOUT_H) * magAdjVals[2] + axisShift[2];	// Reads Z-axis of Magnetometer and performs corrections on it
			break;
		}
		delayMicroseconds(100);
	}
}

void readTemp(){	// Takes a temperature reading from the temperature sensor
	temp = readSensor(MPU_ADDR, TEMP_OUT_L, TEMP_OUT_H);	// Reads temperature data
	tempInC = (((float)temp)/340 + 35);
	tempInF = tempInC * 1.8 + 32;
}

void updateSensors(){	// Takes readings from all of the sensors
	readAccel();	// Accelerometer
	readGyro();		// Gyroscope
	readMag();		// Magnetometer
	readTemp();		// Temperature
}

int readSensor(byte DEV_ADDR, byte REG_L, byte REG_H){	// Reads a sensors LSB and HSB and returns a combined 16-bit signed int
	return ((int16_t)readByte(DEV_ADDR, REG_H) << 8) | readByte(DEV_ADDR, REG_L);
}