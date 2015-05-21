#include "utility/twi.h"
#include "Wire.h"
#include "Servo.h"

float pose[3];
float accelInG[3];
boolean heartbeat = false;
int cycleswoheartbeat = 0;

void setup(){
	Wire.begin();
	Serial1.begin(9600);
	while(!initSensors()){
		delay(500);
		Serial1.println("Error initializing the sensors.");
	}
	initMotors();
}

void loop(){
	if(cycleswoheartbeat > 5){
		MotorLoop(true);
		exit(0);
	}
	sensorLoop();
	MotorLoop(false);
	if(heartbeat)
		cycleswoheartbeat = 0;
	else
		cycleswoheartbeat++;
	heartbeat = false;
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