#include "ArduinoRobot.h"
#include "Arduino.h"


void RobotControl::keyboardCalibrate(int *vals){

  	messageOut1.writeByte(COMMAND_CALIBRATE_KEY);
	for(int i=0;i<5;i++){
		messageOut1.writeInt(vals[i]);
	}
	messageOut1.sendData();
}

int8_t RobotControl::keyboardRead(void){

  	messageOut1.writeByte(COMMAND_AVERAGE_READ_KEY);
	//Serial.println("*************");
	//Serial.print("cmd: ");
	//Serial.println(COMMAND_AVERAGE_READ_KEY);
	messageOut1.sendData();
	delay(10);
	while(!messageIn1.receiveData());
	uint8_t cmd=messageIn1.readByte();
	//Serial.print("cmd: ");
	//Serial.println(cmd);
	if(!(cmd==COMMAND_AVERAGE_READ_KEY_RE))
		return false;
	int8_t dt=messageIn1.readByte();
	//Serial.print("aver: ");
	//Serial.println(dt);
	return dt;
}
