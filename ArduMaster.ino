#include "Servo.h"
#include "utility/twi.h"
#include "Wire.h"

#define MPU_ADDR		104
#define MAG_ADDR		12

//Definde MAG Registers
#define MAG_WHO_AM_I	0x00	//Returns 0x48
#define MAG_INFO		0x01	//Does Nothing?
#define MAG_STATUS_1	0x02	//Val of 1 = ready, 0 = normal, Data ready status bit 0
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08
#define MAG_STATUS_2	0x09	//Val of 1 = error, 0 = normal, magnetic sensor overflow bit 3 and data read error bit 2
#define MAG_CONTROL		0x0A	//Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define MAG_SELF_TEST	0x0C	//Write 1 to enable self test on bit 6
#define MAG_SENS_ADJ_X	0x01	//Fuse ROM X-axis sensitivity adjustment value
#define MAG_SENS_ADJ_Y	0x11	//Fuse ROM Y-axis sensitivity adjustment value
#define MAG_SENS_ADJ_Z	0x12	//Fuse ROM Z-axis sensitivity adjustment value

//Define Registers
#define INT_PIN_CFG		55
#define USER_CTRL		106
#define PWR_MGMT_1		107
#define PWR_MGMT_2		108
#define WHO_AM_I		117
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

void setup(){
	Wire.begin();
	Serial.begin(9600);
	if(!initSensors()){
		while(1){
			delay(500);
			Serial.println("error");
		}
	}
	//initMotors();
}

void loop(){
	delay(1000);
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