#include "utility/twi.h"
#include "Wire.h"
#include "Servo.h"

#define MOTORS_ENABLED false
#define HEARTBEATS_ENABLED false

float pose[3];
int cycleswoheartbeat = -20;

int packets = 0;
long oldTime;
long newTime;

void setup(){
	Wire.begin();
	Serial.begin(57600);
	while(!initSensors()){
		delay(500);
		Serial.println("Error initializing the sensors.");
	}
	if(MOTORS_ENABLED)
		initMotors();
	oldTime = micros();
}

void loop(){
	sensorLoop();
	/*newTime = micros();
	if (newTime - oldTime > 1000000){
		Serial.println(packets / ((newTime - oldTime) / 1000000.0));
		oldTime = newTime;
		packets = 0;
	}

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
		MotorLoop();*/
}

byte readByte(int DEV_ADDR, byte REG){	// Reads a byte from the given I2C device and register
	byte Byte;
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(REG);
	Wire.endTransmission(false);
	Wire.requestFrom(DEV_ADDR, 1);
	while(Wire.available())
	Byte = Wire.read();
	return Byte;
}

void writeByte(byte DEV_ADDR, byte REG, byte VAL){	// Writes a byte to the given I2C device and register
	Wire.beginTransmission(DEV_ADDR);
	Wire.write(REG);
	Wire.write(VAL);
	Wire.endTransmission();
}
