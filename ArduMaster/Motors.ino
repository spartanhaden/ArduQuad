//Motors
//1 - 44
//2 - 43
//3 - 12
//4 - 13
//5 - 8
//6 - 7
//7 - 3
//8 - 2

Servo m1;
Servo m2;
Servo m3;
Servo m4;

void initMotors(){
  m1.attach(44);
  m2.attach(43);
  m3.attach(12);
  m4.attach(13);
  setAllMotors(1500);
  delay(5000);
  setAllMotors(1000);
}

void motorLoop(){
  setAllMotors(1700);
  delay(1000);
  setAllMotors(1000);
  delay(5000);
}

void setAllMotors(int speed){
  m1.writeMicroseconds(speed);
  m2.writeMicroseconds(speed);
  m3.writeMicroseconds(speed);
  m4.writeMicroseconds(speed);
}
