#include <Wire.h>
#define DEV_ADDR  0x68
void setup(){
  Wire.begin();
  Serial.begin(9600);
  config();
}

void loop(){
  printVals();
  delay(500);
}

void config(){
  writeByte(0x6B, 43);
  delay(100);
}

void printVals(){
  Serial.println(readByte(0x75));
}

byte readByte(byte reg){
  byte bite;
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.beginTransmission(DEV_ADDR);
  Wire.requestFrom(DEV_ADDR, 1);
  while(Wire.available())
    bite = Wire.read();
  return bite;
}

void writeByte(byte reg, byte val){
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}
