#include "Servo.h"
#include "utility/twi.h"
#include "Wire.h"

#define MPU_ADDR		0x68
#define MAG_ADDR		0x0E

//Define Registers
#define WHO_AM_I		0x75
#define PWR_MGMT_1		0x6B
#define PWR_MGMT_2		0x6C
//Define ACCEL Registers
#define ACCEL_XOUT_H	0x3B
#define ACCEL_XOUT_L	0x3C
#define ACCEL_YOUT_H	0x3D
#define ACCEL_YOUT_L	0x3E
#define ACCEL_ZOUT_H	0x3F
#define ACCEL_ZOUT_L	0x40
//Define TEMP Registers
#define TEMP_OUT_H		0x41
#define TEMP_OUT_L		0x42
//Define GYRO Registers
#define GYRO_XOUT_H		0x43
#define GYRO_XOUT_L		0x44
#define GYRO_YOUT_H		0x45
#define GYRO_YOUT_L		0x46
#define GYRO_ZOUT_H		0x47
#define GYRO_ZOUT_L		0x48

//Servo motor;

void setup(){
	Wire.begin();
	Serial.begin(9600);
	initAccel();
	//initMag();
	//initMotors();
}

void loop(){
	delay(500);
	AccelLoop();
	//MagnoLoop();
	//MotorLoop();
}

byte readByte(int DEV_ADDR, byte REG){
	byte Byte;
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(REG);
	Wire.endTransmission(false);
	Wire.requestFrom(DEV_ADDR, 1);
	while(Wire.available())
	Byte = Wire.read();
	return Byte;
}

void writeByte(byte DEV_ADDR, byte REG, byte VAL){
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(REG);
	Wire.write(VAL);
	Wire.endTransmission();
}