//Motors

Servo m1;
Servo m2;
Servo m3;
Servo m4;

void initMotors(){
  m1.attach(8);
  m2.attach(7);
  m3.attach(3);
  m4.attach(2);
  setAllMotors(1500);
  delay(5000);
  setAllMotors(1000);
}

void motorLoop(){
  setAllMotors(1500);
  delay(100);
  setAllMotors(1000);
  delay(1000);
}

void setAllMotors(int speed){
  m1.writeMicroseconds(speed);
  m2.writeMicroseconds(speed);
  m3.writeMicroseconds(speed);
  m4.writeMicroseconds(speed);
}
