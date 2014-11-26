#include "Servo.h"
#include "utility/twi.h"
#include "Wire.h"

#define MPU_ADDR		104
#define MAG_ADDR		14

//Define Registers
#define WHO_AM_I		117
#define PWR_MGMT_1		107
#define PWR_MGMT_2		108
//Define ACCEL Registers
#define ACCEL_XOUT_H	59
#define ACCEL_XOUT_L	60
#define ACCEL_YOUT_H	61
#define ACCEL_YOUT_L	62
#define ACCEL_ZOUT_H	63
#define ACCEL_ZOUT_L	64
//Define TEMP Registers
#define TEMP_OUT_H		65
#define TEMP_OUT_L		66
//Define GYRO Registers
#define GYRO_XOUT_H		67
#define GYRO_XOUT_L		68
#define GYRO_YOUT_H		69
#define GYRO_YOUT_L		70
#define GYRO_ZOUT_H		71
#define GYRO_ZOUT_L		72

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