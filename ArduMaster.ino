//#include "Servo.h"
#include "utility/twi.h"
#include "Wire.h"
#include "Servo.h"

#define PI	3.14159265359

float accelInG[3];

void setup(){
	Wire.begin();
	Serial1.begin(9600);
	while(!initSensors()){
		delay(500);
		Serial.println("Error initializing the sensors.");
	}
	//initMotors();
}

void loop(){
	sensorLoop();
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