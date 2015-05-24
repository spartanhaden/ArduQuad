// Motors
// ArduPilot output - Arduino pin number
// 1 - 44	broken
// 2 - 43
// 3 - 12
// 4 - 13
// 5 - 8 - BACK_MOTOR_PIN
// 6 - 7 - LEFT_MOTOR_PIN
// 7 - 3 - FRONT_MOTOR_PIN
// 8 - 2 - RIGHT_MOTOR_PIN

// The motor pins with an associated direction
const int RIGHT_MOTOR_PIN	= 2;
const int FRONT_MOTOR_PIN	= 3;
const int LEFT_MOTOR_PIN	= 7;
const int BACK_MOTOR_PIN	= 8;

// Min and Max signal for the ESCs
const int MIN_SIGNAL		= 700;
const int MAX_SIGNAL		= 2000;

// The servos that will be used to manipulate the ESCs
Servo RIGHT_MOTOR;
Servo FRONT_MOTOR;
Servo LEFT_MOTOR;
Servo BACK_MOTOR;

void initMotors(){	// Initializes the ESCs
	RIGHT_MOTOR.attach(RIGHT_MOTOR_PIN);
	//FRONT_MOTOR.attach(FRONT_MOTOR_PIN);
	LEFT_MOTOR.attach(LEFT_MOTOR_PIN);
	//BACK_MOTOR.attach(BACK_MOTOR_PIN);
	setAllMotors(MIN_SIGNAL);
	delay(10000);
}

void MotorLoop(){	// Updates the motor output values
	angle();
}

void killMotors(){
	setAllMotors(MIN_SIGNAL);
}

void angle(){	// Simple stabilization test
	RIGHT_MOTOR.writeMicroseconds(constrain(MIN_SIGNAL + 100 + pose[1] - 90.0, 700, 900));
	//FRONT_MOTOR.writeMicroseconds(min(MIN_SIGNAL + max(25 * accelInG[1], 0), 1200));
	LEFT_MOTOR.writeMicroseconds(constrain(MIN_SIGNAL + 100 + (pose[1] * -1.0) + 90.0, 700, 900));
	//BACK_MOTOR.writeMicroseconds(min(MIN_SIGNAL + max(25 * -accelInG[1], 0), 1200));
}

void setAllMotors(int speed){	// Sets all of the motors to the speed passed in
	RIGHT_MOTOR.writeMicroseconds(speed);
	//FRONT_MOTOR.writeMicroseconds(speed);
	LEFT_MOTOR.writeMicroseconds(speed);
	//BACK_MOTOR.writeMicroseconds(speed);
}