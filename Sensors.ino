int accel[3];
int gyro[3];
int mag[3];
int temp;

void initSensors(){
	writeByte(MPU_ADDR, PWR_MGMT_1, 128);	//PWR_MGMT_1 send 128, reset device
	delay(5);
	writeByte(MPU_ADDR, PWR_MGMT_1, 1);		//PWR_MGMT_1 send 1, set clock source to X axis gyroscope and set sensor active
	delay(5);
}

void sensorLoop(){
	if(readByte(MPU_ADDR,PWR_MGMT_1) > 1) initSensors();
	updateSensors();
	printSensorValues();
}

void printSensorValues(){
	Serial.print("A X: ");
	Serial.print(accel[0]);
	Serial.print("\tA Y: ");
	Serial.print(accel[1]);
	Serial.print("\tA Z: ");
	Serial.println(accel[2]);
	Serial.print("G X: ");
	Serial.print(gyro[0]);
	Serial.print("\tG Y: ");
	Serial.print(gyro[1]);
	Serial.print("\tG Z: ");
	Serial.println(gyro[2]);
	//Serial.print("M X: ");
	//Serial.print(mag[0]);
	//Serial.print("\tM Y: ");
	//Serial.print(mag[1]);
	//Serial.print("\tM Z: ");
	//Serial.println(mag[2]);
	Serial.print("T: ");
	Serial.println(temp);
	Serial.println();
}

void updateSensors(){
	accel[0] = readSensor(MPU_ADDR, ACCEL_XOUT_L, ACCEL_XOUT_H);
	accel[1] = readSensor(MPU_ADDR, ACCEL_YOUT_L, ACCEL_YOUT_H);
	accel[2] = readSensor(MPU_ADDR, ACCEL_ZOUT_L, ACCEL_ZOUT_H);
	gyro[0] = readSensor(MPU_ADDR, GYRO_XOUT_L, GYRO_XOUT_H);
	gyro[1] = readSensor(MPU_ADDR, GYRO_YOUT_L, GYRO_YOUT_H);
	gyro[2] = readSensor(MPU_ADDR, GYRO_ZOUT_L, GYRO_ZOUT_H);
	//mag[0] = readSensor(MPU_ADDR);
	//mag[1] = readSensor(MPU_ADDR);
	//mag[2] = readSensor(MPU_ADDR);
	temp = readSensor(MPU_ADDR, TEMP_OUT_L, TEMP_OUT_H);
}

int readSensor(byte DEV_ADDR, byte REG_L, byte REG_H){
	return (int)((readByte(DEV_ADDR, REG_H) << 8) + readByte(DEV_ADDR, REG_L));
}