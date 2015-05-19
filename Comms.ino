void serialEvent(){
	while(Serial.available()){
	// statement
	}
}

void sendPacket(){	// Sends a heartbeat to the other device
	Serial.print(pose[0]);
	Serial.print(',');
	Serial.print(pose[1]);
	Serial.print(',');
	Serial.print(pose[2]);
	Serial.print('n');
}