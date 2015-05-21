void serialEvent1(){
	while(Serial1.available())
		if(Serial1.read() == 'a')
			heartbeat = true;
}

void sendPacket(){	// Sends a heartbeat to the other device
	Serial1.print(pose[0]);
	Serial1.print(',');
	Serial1.print(pose[1]);
	Serial1.print(',');
	Serial1.println(pose[2]);
}