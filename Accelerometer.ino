void initAccel(){
	writeByte(MPU_ADDR, PWR_MGMT_1, 128);	//PWR_MGMT_1 send 128, reset device
	delay(5);
	writeByte(MPU_ADDR, PWR_MGMT_1, 1);		//PWR_MGMT_1 send 1, set clock source to X axis gyroscope and set sensor active
	delay(5);
}

void AccelLoop(){
	if(readByte(MPU_ADDR,PWR_MGMT_1) > 1) initAccel();
	printAccelVals();
}

void printAccelVals(){
	Serial.print("A X: ");
	Serial.print(readSensor(MPU_ADDR, ACCEL_XOUT_L, ACCEL_XOUT_H));
	Serial.print("\tA Y: ");
	Serial.print(readSensor(MPU_ADDR, ACCEL_YOUT_L, ACCEL_YOUT_H));
	Serial.print("\tA Z: ");
	Serial.println(readSensor(MPU_ADDR, ACCEL_ZOUT_L, ACCEL_ZOUT_H));
	Serial.print("G X: ");
	Serial.print(readSensor(MPU_ADDR, GYRO_XOUT_L, GYRO_XOUT_H));
	Serial.print("\tG Y: ");
	Serial.print(readSensor(MPU_ADDR, GYRO_YOUT_L, GYRO_YOUT_H));
	Serial.print("\tG Z: ");
	Serial.println(readSensor(MPU_ADDR, GYRO_ZOUT_L, GYRO_ZOUT_H));
	Serial.println();
}

int readSensor(byte DEV_ADDR, byte REG_L, byte REG_H){
	return (int)((readByte(DEV_ADDR, REG_H) << 8) + readByte(DEV_ADDR, REG_L));
}