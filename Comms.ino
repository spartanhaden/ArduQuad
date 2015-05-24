void serialEvent(){
	while(Serial.available())
		if(HEARTBEATS_ENABLED)
			if(Serial.read() == 'a')
				cycleswoheartbeat = 0;
}

void sendPacket(){	// Sends a heartbeat to the other device
	Serial.print(pose[0]);
	Serial.print(',');
	Serial.print(pose[1]);
	Serial.print(',');
	Serial.println(pose[2]);
}