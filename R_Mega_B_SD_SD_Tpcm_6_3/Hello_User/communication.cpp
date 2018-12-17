#include "ArduinoRobot.h"

bool RobotControl::isActionDone(){

	if(messageIn2.receiveData()){
		if(messageIn2.readByte()==COMMAND_ACTION_DONE){
			return true;
		}
	}
	return false;
}

void RobotControl::pauseMode(uint8_t onOff){

	messageOut2.writeByte(COMMAND_PAUSE_MODE);
	if(onOff){
		messageOut2.writeByte(true);
	}else{
		messageOut2.writeByte(false);
	}
	messageOut2.sendData();
}

void RobotControl::lineFollowConfig(uint8_t KP, uint8_t KD, uint8_t robotSpeed, uint8_t intergrationTime){

	messageOut2.writeByte(COMMAND_LINE_FOLLOW_CONFIG);
	messageOut2.writeByte(KP);
	messageOut2.writeByte(KD);
	messageOut2.writeByte(robotSpeed);
	messageOut2.writeByte(intergrationTime);
	messageOut2.sendData();
}
