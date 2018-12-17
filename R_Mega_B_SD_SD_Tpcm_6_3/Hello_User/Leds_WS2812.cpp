
#include "ArduinoRobot.h"


void RobotControl::leds_Show(uint8_t style, uint8_t ncycleInStyle, uint8_t ncycleAllStyles, uint16_t wait){
	messageOut2.writeByte(COMMAND_LEDS_SHOW);	
	messageOut2.writeByte(style);
	messageOut2.writeByte(ncycleInStyle);
	messageOut2.writeByte(ncycleAllStyles);
	messageOut2.writeInt(wait);	
	messageOut2.sendData();
}

uint8_t RobotControl::leds_Is_Show(){
	messageOut2.writeByte(COMMAND_LEDS_IS_SHOW);	
	messageOut2.sendData();
	delay(10);
	while(!messageIn2.receiveData());
	byte cmd=messageIn2.readByte();
	if(!(cmd==COMMAND_LEDS_IS_SHOW_RE))
		return false;
	byte dt=messageIn2.readByte();
	return dt;
}

void RobotControl::leds_Stop(){
	messageOut2.writeByte(COMMAND_LEDS_STOP);	
	messageOut2.sendData();
}


void RobotControl::leds_Pause_Show(){
	messageOut2.writeByte(COMMAND_PAUSE_SHOW_LEDS);	
	messageOut2.sendData();
}
