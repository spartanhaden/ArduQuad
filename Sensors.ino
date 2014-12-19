int accel[3];
int gyro[3];
int mag[3];
int temp;
float tempInC;
float tempInF;

boolean initSensors(){
	writeByte(MPU_ADDR, PWR_MGMT_1, 128);	//PWR_MGMT_1 send 128, reset device
	delay(100);
	writeByte(MPU_ADDR, PWR_MGMT_1, 1);		//PWR_MGMT_1 send 1, set clock source to X axis gyroscope and set sensor active
	delay(200);
	writeByte(MPU_ADDR, INT_PIN_CFG, 2);	//Enable I2C passthrough
	delay(5);
	if(readByte(MPU_ADDR, WHO_AM_I) != 104) return false;
	else if(readByte(MAG_ADDR, MAG_WHO_AM_I) != 72) return false;
	else if(!magSelfTest()) return false;
	return true;
}

void sensorLoop(){
	if(readByte(MPU_ADDR,PWR_MGMT_1) > 1) initSensors();	//Checks for disconnection of Sensor
	updateSensors();
	//printSensorValues();
	printSensorForProcessing();
}

boolean magSelfTest(){
	int magSelfTest[3];
	boolean testPassed = true;
	writeByte(MAG_ADDR, MAG_CONTROL, 0);	//Set Power-down mode
	writeByte(MAG_ADDR, MAG_SELF_TEST, 64);	//Write “1” to SELF bit of self-test register
	writeByte(MAG_ADDR, MAG_CONTROL, 8);	//Set Self-test Mode
	for(int i = 0; i < 100; i++){			//Waits 10 ms for the Magnetometer data to become available
		if(magDataReady()){
			break;
		}
		delayMicroseconds(100);
	}
	if(!magDataReady()){
		testPassed = false;	//Check Data Ready
		}else{
		magSelfTest[0] = readSensor(MAG_ADDR, MAG_XOUT_L, MAG_XOUT_H);				//Get X self-test value
		magSelfTest[1] = readSensor(MAG_ADDR, MAG_YOUT_L, MAG_YOUT_H);				//Get Y self-test value
		magSelfTest[2] = readSensor(MAG_ADDR, MAG_ZOUT_L, MAG_ZOUT_H);				//Get Z self-test value
		if(magSelfTest[0] < -100 || magSelfTest[0] > 100) testPassed = false;		//Test X self-test value
		else if(magSelfTest[1] < -100 || magSelfTest[1] > 100) testPassed = false;	//Test Y self-test value
		else if(magSelfTest[2] < -1000 || magSelfTest[2] > -300)testPassed = false;	//Test Z self-test value
	}
	writeByte(MAG_ADDR, MAG_SELF_TEST, 0);	//Write “0” to SELF bit of self-test register
	return testPassed;
}

boolean magDataReady(){
	return readByte(MAG_ADDR, MAG_STATUS_1) == 1;
}

void printSensorForProcessing(){
	Serial.print(float(accel[0])/16384);
	Serial.print(",");
	Serial.print(float(accel[1])/16384);
	Serial.print(",");
	Serial.println(float(accel[2])/16384);
}

void printSensorValues(){
	Serial.print("A X: ");
	Serial.print(float(accel[0])/16384);
	Serial.print("\tA Y: ");
	Serial.print(float(accel[1])/16384);
	Serial.print("\tA Z: ");
	Serial.println(float(accel[2])/16384);
	Serial.print("G X: ");
	Serial.print(gyro[0]);
	Serial.print("\tG Y: ");
	Serial.print(gyro[1]);
	Serial.print("\tG Z: ");
	Serial.println(gyro[2]);
	Serial.print("M X: ");
	Serial.print(mag[0]);
	Serial.print("\tM Y: ");
	Serial.print(mag[1]);
	Serial.print("\tM Z: ");
	Serial.println(mag[2]);
	Serial.print("T: ");
	Serial.print(tempInC);
	Serial.print(" C\t");
	Serial.print(tempInF);
	Serial.println(" F");
	Serial.println();
}

void updateSensors(){
	writeByte(MAG_ADDR, MAG_CONTROL, 1);
	accel[0] = readSensor(MPU_ADDR, ACCEL_XOUT_L, ACCEL_XOUT_H);
	accel[1] = readSensor(MPU_ADDR, ACCEL_YOUT_L, ACCEL_YOUT_H);
	accel[2] = readSensor(MPU_ADDR, ACCEL_ZOUT_L, ACCEL_ZOUT_H);
	gyro[0] = readSensor(MPU_ADDR, GYRO_XOUT_L, GYRO_XOUT_H);
	gyro[1] = readSensor(MPU_ADDR, GYRO_YOUT_L, GYRO_YOUT_H);
	gyro[2] = readSensor(MPU_ADDR, GYRO_ZOUT_L, GYRO_ZOUT_H);
	delay(10);
	for(int i = 0; i < 100; i++){	//Waits 10 ms for the Magnetometer data to become available
		if(magDataReady()){
			break;
		}
		delayMicroseconds(100);
	}
	mag[0] = readSensor(MAG_ADDR, MAG_XOUT_L, MAG_XOUT_H);
	mag[1] = readSensor(MAG_ADDR, MAG_YOUT_L, MAG_YOUT_H);
	mag[2] = readSensor(MAG_ADDR, MAG_ZOUT_L, MAG_ZOUT_H);
	temp = readSensor(MPU_ADDR, TEMP_OUT_L, TEMP_OUT_H);
	tempInC = (((float)temp)/340 + 35);
	tempInF = tempInC * 1.8 + 32;
}

int readSensor(byte DEV_ADDR, byte REG_L, byte REG_H){
	return ((int16_t)readByte(DEV_ADDR, REG_H) << 8) | readByte(DEV_ADDR, REG_L);
}