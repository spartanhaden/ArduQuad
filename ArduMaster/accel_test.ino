#define ACCEL_ADDR  0x68

void loop(){
  printVals();
  delay(500);
}

void initAccel(){
  writeByte(ACCEL_ADDR, 0x6B, 43);
  delay(100);
}

void printVals(){
  Serial.println(readByte(ACCEL_ADDR, 0x75));
}
