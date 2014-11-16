#include "Servo.h"
#include "utility/twi.h"
#include "Wire.h"

#define ACCEL_ADDR	0x68
#define MAG_ADDR	0x0E

//Servo motor;
//String line = "";
//boolean endLine = false;

void setup(){
	Wire.begin();
	Serial.begin(9600);
	initAccel();
	//initMag();
	//initMotors();
}

void loop(){
	Serial.println("hello");
	AccelLoop();
	//MagnoLoop();
	//MotorLoop();
}

byte readByte(int DEV_ADDR, byte REG){
	byte Byte;
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(REG);
	Wire.endTransmission();				//skip this line?
	Wire.beginTransmission(DEV_ADDR);	//skip this line?
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