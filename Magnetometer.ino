int axis[3];

void initMag(){
	
}

void MagnoLoop(){
	updateAxes();
	printVals();
}

void printMagVals(){
	Serial.print("M X: ");
	Serial.print(axis[0]);
	Serial.print("\tM Y: ");
	Serial.print(axis[1]);
	Serial.print("\tM Z: ");
	Serial.println(axis[2]);
}

void updateAxes(){
	axis[0] = readSensor();
	axis[1] = readSensor(0x03);
	axis[2] = readSensor(0x05);
}