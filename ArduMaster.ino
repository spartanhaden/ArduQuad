#include "utility/twi.h"
#include "Wire.h"
#include "Servo.h"

const boolean MOTORS_ENABLED = false;
const boolean HEARTBEATS_ENABLED = false;

float pose[3];
int cycleswoheartbeat = -20;

void setup(){
	Wire.begin();
	Serial.begin(57600);
	while(!initSensors()){
		delay(500);
		Serial.println("Error initializing the sensors.");
	}
	if(MOTORS_ENABLED)
		initMotors();
}

void loop(){
	if(HEARTBEATS_ENABLED){			// Use heartbeats to detect disconnect of radio
		if(cycleswoheartbeat > 10){	// Not enough heartbeats, exit
			if(MOTORS_ENABLED)
				killMotors();
			exit(0);
		}
		cycleswoheartbeat++;
	}
	sensorLoop();
	if(MOTORS_ENABLED)
		MotorLoop();
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