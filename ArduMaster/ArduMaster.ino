//Quadrotor

#include <Servo.h>
#include <Wire.h>

void setup(){
  Wire.begin();
  Serial.begin(9600);
  //initMag();
  initAccel();
  //initMotors();
}

void loop(){
  Accelloop();
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
