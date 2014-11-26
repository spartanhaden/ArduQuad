void initAccel(){
	writeByte(MPU_ADDR, PWR_MGMT_1, 0x80);	//PWR_MGMT_1 send 0x80, reset device
	delay(5);
	writeByte(MPU_ADDR, PWR_MGMT_1, 0x00);	//PWR_MGMT_1 send 0x00, active mode
	delay(5);
}

void AccelLoop(){
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