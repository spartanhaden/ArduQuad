#include <Servo.h>
#include <Wire.h>

//Quadrotor
//int pin = 8;
Servo m1;
Servo m2;
Servo m3;
Servo m4;
void setup(){
  Wire.begin();
  Serial.begin(9600);
  initMag();
  initAccel();
  m1.attach(8);
  m2.attach(7);
  m3.attach(3);
  m4.attach(2);
  setSpeed(1500);
  delay(5000);
  setSpeed(1000);
}
void loop(){
  setSpeed(1500);
  delay(100);
  setSpeed(1000);
  delay(1000);
}
void setSpeed(int speed){
  m1.writeMicroseconds(speed);
  m2.writeMicroseconds(speed);
  m3.writeMicroseconds(speed);
  m4.writeMicroseconds(speed);
}

byte readByte(int DEV_ADDR, byte REG){
  byte Byte;
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(REG);
  Wire.endTransmission();
  Wire.beginTransmission(DEV_ADDR);
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
