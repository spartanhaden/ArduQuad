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
	Serial.print("ACEL X: ");
	Serial.print(readByte(MPU_ADDR, ACCEL_XOUT_L)|(readByte(MPU_ADDR, ACCEL_XOUT_H) << 8));
	Serial.print("\tACEL Y: ");
	Serial.print(readByte(MPU_ADDR, ACCEL_YOUT_L)|(readByte(MPU_ADDR, ACCEL_YOUT_H) << 8));
	Serial.print("\tACEL Z: ");
	Serial.println(readByte(MPU_ADDR, ACCEL_ZOUT_L)|(readByte(MPU_ADDR, ACCEL_ZOUT_H) << 8));
	Serial.print("GYRO X: ");
	Serial.print(readByte(MPU_ADDR, GYRO_XOUT_L)|(readByte(MPU_ADDR, GYRO_XOUT_H) << 8));
	Serial.print("\tGYRO Y: ");
	Serial.print(readByte(MPU_ADDR, GYRO_YOUT_L)|(readByte(MPU_ADDR, GYRO_YOUT_H) << 8));
	Serial.print("\tGYRO Z: ");
	Serial.println(readByte(MPU_ADDR, GYRO_ZOUT_L)|(readByte(MPU_ADDR, GYRO_ZOUT_H) << 8));
	Serial.println();
}