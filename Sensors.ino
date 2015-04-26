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
	writeByte(MPU_ADDR, PWR_MGMT_1, 1);		// PWR_MGMT_1 send 1, set clock source to X axis gyroscope and set sensor active
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
	if(readByte(MPU_ADDR,PWR_MGMT_1) > 1) initSensors();	// Checks for disconnection of Sensor
	//updateSensors();
	readAccel();
	readGyro();
	calculateAngles();
	//printSensorForAndroid();
	//printSensorValues();
	//printSensorForProcessing();
}

void calculateAngles(){
	accelInG[0] = accel[0] / 16384.0;
	accelInG[1] = accel[1] / 16384.0;
	accelInG[2] = accel[2] / 16384.0;
	float R = sqrt(pow(accelInG[0], 2) + pow(accelInG[1], 2) + pow(accelInG[2], 2));
	accelInG[0] = asin(accelInG[0] / R) * (180.0 / PI);
	accelInG[1] = asin(accelInG[1] / R) * (180.0 / PI);
	accelInG[2] = asin(accelInG[2] / R) * (180.0 / PI);
}

boolean magSelfTest(){
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

void setMagAdjValues(){
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

/*void printSensorForProcessing(){	// Prints sensor info for use with a processing sketch I have made
	Serial.print(Rx);
	Serial.print(",");
	Serial.print(Ry);
	Serial.print(",");
	Serial.print(Rz);
	Serial.print("n");
}*/

void printSensorForAndroid(){	// Prints sensor info for use with an Android app I have made
	Serial.write(0x02);
	Serial.write(0x01);
	Serial.write(0x05);
}

void printSensorValues(){	// Prints human readable sensor information
	Serial.print("Accelerometer X: ");
	Serial.print(float(accel[0])/16384);
	Serial.print("\tY: ");
	Serial.print(float(accel[1])/16384);
	Serial.print("\tZ: ");
	Serial.println(float(accel[2])/16384);
	Serial.print("Gyroscope X: ");
	Serial.print(gyro[0]);
	Serial.print("\tY: ");
	Serial.print(gyro[1]);
	Serial.print("\tZ: ");
	Serial.println(gyro[2]);
	Serial.print("Magnetometer X: ");
	Serial.print(mag[0]);
	Serial.print("\tY: ");
	Serial.print(mag[1]);
	Serial.print("\tZ: ");
	Serial.println(mag[2]);
	Serial.print("Temperature: ");
	Serial.print(tempInC);
	Serial.print(" C\t");
	Serial.print(tempInF);
	Serial.println(" F");
	Serial.println();
}

void readAccel(){
	accel[0] = readSensor(MPU_ADDR, ACCEL_XOUT_L, ACCEL_XOUT_H);	// Reads X-axis of Accelerometer
	accel[1] = readSensor(MPU_ADDR, ACCEL_YOUT_L, ACCEL_YOUT_H);	// Reads Y-axis of Accelerometer
	accel[2] = readSensor(MPU_ADDR, ACCEL_ZOUT_L, ACCEL_ZOUT_H);	// Reads Z-axis of Accelerometer
}

void readGyro(){
	gyro[0] = readSensor(MPU_ADDR, GYRO_XOUT_L, GYRO_XOUT_H);		// Reads X-axis of Gyroscope
	gyro[1] = readSensor(MPU_ADDR, GYRO_YOUT_L, GYRO_YOUT_H);		// Reads Y-axis of Gyroscope
	gyro[2] = readSensor(MPU_ADDR, GYRO_ZOUT_L, GYRO_ZOUT_H);		// Reads Z-axis of Gyroscope
}

void readMag(){
	writeByte(MAG_ADDR, MAG_CONTROL, 1);	// Sets the Magnetometer to single measurement mode
	for(int i = 0; i < 100; i++){			// Waits up to 10 ms for the Magnetometer data to become available should take 0.3 ms
		if(magDataReady()){
			mag[0] = (int) readSensor(MAG_ADDR, MAG_XOUT_L, MAG_XOUT_H) * magAdjVals[0] + axisShift[0];	// Reads X-axis of Magnetometer
			mag[1] = (int) readSensor(MAG_ADDR, MAG_YOUT_L, MAG_YOUT_H) * magAdjVals[1] + axisShift[1];	// Reads Y-axis of Magnetometer
			mag[2] = (int) readSensor(MAG_ADDR, MAG_ZOUT_L, MAG_ZOUT_H) * magAdjVals[2] + axisShift[2];	// Reads Z-axis of Magnetometer
			break;
		}
		delayMicroseconds(100);
	}
}

void readTemp(){
	temp = readSensor(MPU_ADDR, TEMP_OUT_L, TEMP_OUT_H);	// Reads temperature data
	tempInC = (((float)temp)/340 + 35);
	tempInF = tempInC * 1.8 + 32;
}

void updateSensors(){	// Takes readings from the sensors
	readAccel();
	readGyro();
	readMag();
	readTemp();
}

int readSensor(byte DEV_ADDR, byte REG_L, byte REG_H){	// Reads a sensors LSB and HSB and returns a combined 16-bit signed int
	return ((int16_t)readByte(DEV_ADDR, REG_H) << 8) | readByte(DEV_ADDR, REG_L);
}