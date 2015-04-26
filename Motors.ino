// Motors
// ArduPilot output - Arduino pin number
// 1 - 44	broken
// 2 - 43
// 3 - 12
// 4 - 13
// 5 - 8
// 6 - 7
// 7 - 3
// 8 - 2

Servo RIGHT_MOTOR;
Servo FRONT_MOTOR;
Servo LEFT_MOTOR;
Servo BACK_MOTOR;

void initMotors(){
	RIGHT_MOTOR.attach(RIGHT_MOTOR_PIN);
	FRONT_MOTOR.attach(FRONT_MOTOR_PIN);
	LEFT_MOTOR.attach(LEFT_MOTOR_PIN);
	BACK_MOTOR.attach(BACK_MOTOR_PIN);
	setAllMotors(MIN_SIGNAL);
	delay(10000);
}

void MotorLoop(){
	angle();
	//setAllMotors(1200);
	//delay(500);
	//setAllMotors(MIN_SIGNAL);
	//delay(5000);
}

void angle(){
	RIGHT_MOTOR.writeMicroseconds(MIN_SIGNAL + max(25 * accelInG[0], 0));
	LEFT_MOTOR.writeMicroseconds(MIN_SIGNAL + max(25 * -accelInG[0], 0));
	FRONT_MOTOR.writeMicroseconds(MIN_SIGNAL + max(25 * accelInG[1], 0));
	BACK_MOTOR.writeMicroseconds(MIN_SIGNAL + max(25 * -accelInG[1], 0));
}

void setAllMotors(int speed){
	RIGHT_MOTOR.writeMicroseconds(speed);
	FRONT_MOTOR.writeMicroseconds(speed);
	LEFT_MOTOR.writeMicroseconds(speed);
	BACK_MOTOR.writeMicroseconds(speed);
}