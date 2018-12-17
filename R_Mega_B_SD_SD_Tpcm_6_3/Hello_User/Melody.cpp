#include "ArduinoRobot.h"



extern void Init_PS2();

void RobotControl::playFile_P(const char* filename_P, byte check_isPlAY){
	Serial.println(F("*************"));
	Serial.print(F("playFile, "));
	messageOut1.writeByte(COMMAND_PLAY_FILE);	
	messageOut1.writeStr_P(filename_P);
	Serial.print(F(", "));
	Serial.println(check_isPlAY);
	messageOut1.writeByte(check_isPlAY);
	messageOut1.sendData();
}

void RobotControl::playFile(char* filename, byte check_isPlAY){
	Serial.println(F("*************"));
	Serial.print(F("playFile, "));
	messageOut1.writeByte(COMMAND_PLAY_FILE);	
	messageOut1.writeStr(filename);
	Serial.print(F(", "));
	Serial.println(check_isPlAY);
	messageOut1.writeByte(check_isPlAY);
	messageOut1.sendData();
}

void RobotControl::playFileTime_P(const char* filename_P, int Tsec, byte check_isPlAY){
	messageOut1.writeByte(COMMAND_PLAY_FILE_TIME);	
	messageOut1.writeStr_P(filename_P);
	messageOut1.writeInt(Tsec);	
	messageOut1.writeByte(check_isPlAY);
	messageOut1.sendData();
}

void RobotControl::playFileTime(char* filename, int Tsec, byte check_isPlAY){
	messageOut1.writeByte(COMMAND_PLAY_FILE_TIME);	
	messageOut1.writeStr(filename);
	messageOut1.writeInt(Tsec);	
	messageOut1.writeByte(check_isPlAY);
	messageOut1.sendData();
}

void RobotControl::stopPlayFile(){
	Serial.println(F("*************"));
	Serial.println(F("stopPlayFile"));
	messageOut1.writeByte(COMMAND_STOP_PLAY_FILE);
	messageOut1.sendData();
}

void RobotControl::playFileDisable(){
	Serial.println(F("*************"));
	Serial.println(F("playFileDisable"));
	messageOut1.writeByte(COMMAND_PLAY_DISABLE);	
	messageOut1.sendData();
}

void RobotControl::pausePlayFile(){
	Serial.println(F("*************"));
	Serial.println(F("pausePlayFile"));
	messageOut1.writeByte(COMMAND_PAUSE_PLAY);	
	messageOut1.sendData();
}

void RobotControl::setPlayVolume(byte vol){
	Serial.println(F("*************"));
	Serial.print(F("setPlayVolume, "));
	Serial.println(vol);
	messageOut1.writeByte(COMMAND_SET_VOLUME);	
	messageOut1.writeByte(vol);
	messageOut1.sendData();
}

void RobotControl::changePlayVolume(byte vol){
	Serial.println(F("*************"));
	Serial.print(F("changePlayVolume, "));
	Serial.println(vol);
	messageOut1.writeByte(COMMAND_CHANGE_VOLUME);	
	messageOut1.writeByte(vol);
	messageOut1.sendData();
}

void RobotControl::setPlayQuality(byte qual){
	messageOut1.writeByte(COMMAND_QUALITY_PLAYING);	
	messageOut1.writeByte(qual);
	messageOut1.sendData();
}

void RobotControl::playFileLoop(byte set){
	messageOut1.writeByte(COMMAND_LOOP_PLAY);	
	messageOut1.writeByte(set);
	messageOut1.sendData();
}

boolean RobotControl::isPlaying(){
	messageOut1.writeByte(COMMAND_IS_PLAYING);	
	messageOut1.sendData();
	delay(10);
	while(!messageIn1.receiveData());
	byte cmd=messageIn1.readByte();
	if(!(cmd==COMMAND_IS_PLAYING_RE))
		return false;
	byte dt=messageIn1.readByte();
	return dt;
}

boolean RobotControl::playSoundBuzz(byte nSound){
	Serial.println(F("*************"));
	Serial.print(F("playBuzz, "));
	Serial.println(nSound);
	messageOut1.writeByte(COMMAND_PLAY_BUZZ);	
	messageOut1.writeByte(nSound);
	messageOut1.sendData();
	delay(10);
	while(!messageIn1.receiveData());
	byte cmd=messageIn1.readByte();
	if(!(cmd==COMMAND_PLAY_BUZZ_RE))
		return false;
	byte dt=messageIn1.readByte();
	return dt;
}

