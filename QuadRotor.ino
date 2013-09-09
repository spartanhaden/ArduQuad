#include <Servo.h>

//Quadrotor
//int pin = 8;
Servo m1;
Servo m2;
Servo m3;
Servo m4;
void setup(){
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
