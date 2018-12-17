#include "ArduinoRobotTopBoard.h"
#include "EasyTransfer2.h"
#include "Multiplexer.h"


extern void DoBuzzDuration();
extern bool F_startBuzzDurat;




RobotTopBoard::RobotTopBoard(){
	pinMode(BUZZ_2, OUTPUT);
}


void RobotTopBoard::begin(){
	//initialze communication
	Serial1.begin(9600);
	messageIn.begin(&Serial1);
	messageOut.begin(&Serial1);

	//init MUX
	uint8_t MuxPins[]={MUXA,MUXB,MUXC,MUXD};
	this->Mult.begin(MuxPins,MUX_IN,4);
}


void RobotTopBoard::process(){

	if(F_startBuzzDurat)
		DoBuzzDuration();
}


void RobotTopBoard::parseCommand(){
	uint8_t codename;
	int value;
	if(this->messageIn.receiveData()){
		//Serial.println("data received");
		uint8_t command=messageIn.readByte();
		//Serial.println(command);
		switch(command){
			case COMMAND_ANALOG_WRITE:
				codename=messageIn.readByte();
				value=messageIn.readInt();
				_analogWrite(codename,value);
				break;
			case COMMAND_DIGITAL_WRITE:
				codename=messageIn.readByte();
				value=messageIn.readByte();
				_digitalWrite(codename,value);
				break;
			case COMMAND_ANALOG_READ:
				codename=messageIn.readByte();
				_analogRead(codename);
				break;
			case COMMAND_DIGITAL_READ:
				codename=messageIn.readByte();
				_digitalRead(codename);
				break;
			case COMMAND_ANALOG_READ_KEY:
				_readKeybord();
				break;
			case COMMAND_READ_POT:
				_readKnob();
				break;
			case COMMAND_AVERAGE_READ_KEY:
				_averageReadKeybord();
				break;
			case COMMAND_CALIBRATE_KEY:
				_keybordCalibrate();
				break;
			case COMMAND_PLAY_FILE:
				_playFile();
				break;
			case COMMAND_PLAY_FILE_TIME:
				_playFileTime();
				break;
			case COMMAND_STOP_PLAY_FILE:
				_stopPlayback();
				break;
			case COMMAND_PLAY_DISABLE:
				_disablePlay();
				break;
			case COMMAND_CHANGE_VOLUME:
				_changeVolume();
				break;
			case COMMAND_SET_VOLUME:
				_setVolume();
				break;
			case COMMAND_IS_PLAYING:
				_isPlaying();
				break;
			case COMMAND_PAUSE_PLAY:
				_pausePlay();
				break;
			case COMMAND_QUALITY_PLAYING:
				_setQuality();
				break;
			case COMMAND_LOOP_PLAY:
				_playFileLoop();
				break;
			case COMMAND_PLAY_BUZZ:
				_playSoundBuzz();
				break;
		}
	}
	//delay(5);
}


uint8_t RobotTopBoard::_codenameToPin(uint8_t codename){
	switch(codename){
		case T_TKD0:
			return TKD0;
		case T_TKD1:
			return TKD1;
		case T_TKD3:
			return TKD3;
		case T_TKD4:
			return TKD4;
		case T_TKD5:
			return TKD5;
		case T_LED1:
			return LED1;
	}
}


uint8_t RobotTopBoard::_codenameToAPin(uint8_t codename){
	switch(codename){
		case T_TKD0:
		  return A1;
		case T_TKD1:
		  return A2;
		case T_TKD2:
		  return A3;
		case T_TKD3:
		  return A4;
		case T_TKD4:
		  return A7;
		case T_TKD5:
		  return A11;
	}
}


uint8_t RobotTopBoard::_getTypeCode(uint8_t codename){
	switch(codename){
		case T_TK0:
		case T_TK1:
		case T_TK2:
		case T_TK3:
		case T_TK4:
		case T_TK5:
		case T_TK6:
		case T_TK7:
		case T_BCHRG:
		case T_BON:
			return TYPE_TOP_TK;
			break;
			
		case T_TKD0:
		case T_TKD1:
		case T_TKD2:
		case T_TKD3:
		case T_TKD4:
		case T_TKD5:
		case T_LED1:
			return TYPE_TOP_TKD;
			break;
	}
}


uint8_t RobotTopBoard::_portToTopMux(uint8_t codename){
	switch(codename){
		case T_TK0:
			return 0;
		case T_TK1:
			return 1;
		case T_TK2:
			return 2;
		case T_TK3:
			return 3;
		case T_TK4:
			return 4;
		case T_TK5:
			return 5;
		case T_TK6:
			return 6;
		case T_TK7:
			return 7;
		case T_BCHRG:
			return 8;
		case T_BON:
			return 9;
	}
}


void RobotTopBoard::beginSD(){
	tmrpcm.speakerPin = BUZZ;
	const int chipSelect = 8;  // SD Select pin for Leonardo
	if(!SD.begin(chipSelect)){
		Serial.println("SD fail");
		return;
	}
}


void RobotTopBoard::_digitalReadTopMux(uint8_t codename){
	uint8_t num=_portToTopMux(codename);
	bool value=Mult.getDigitalValueAt(num);
	messageOut.writeByte(COMMAND_DIGITAL_READ_RE);
	messageOut.writeByte(value);
	messageOut.sendData();
}


void RobotTopBoard::_digitalReadTopPin(uint8_t codename){
	uint8_t pin=_codenameToPin(codename);
	bool value=digitalRead(pin);
	messageOut.writeByte(COMMAND_DIGITAL_READ_RE);
	messageOut.writeByte(value);
	messageOut.sendData();
}


void RobotTopBoard::_analogReadTopMux(uint8_t codename){
	uint8_t num=_portToTopMux(codename);
	int value=Mult.getAnalogValueAt(num);
	messageOut.writeByte(COMMAND_ANALOG_READ_RE);
	messageOut.writeInt(value);
	messageOut.sendData();
}


void RobotTopBoard::_analogReadTopPin(uint8_t codename){
	uint8_t pin=_codenameToAPin(codename);
	bool value=analogRead(pin);
	messageOut.writeByte(COMMAND_ANALOG_READ_RE);
	messageOut.writeInt(value);
	messageOut.sendData();
}

//Input and Output ports
void RobotTopBoard::_digitalWrite(uint8_t codename,bool value){
	uint8_t type=_getTypeCode(codename);
	switch(type){
		case TYPE_TOP_TK:
			//Top TKs can't use digitalWrite?
			break;
		case TYPE_TOP_TKD:
			uint8_t pin=_codenameToPin(codename);
			digitalWrite(pin,value); // TKD0-TKD5,LED1
			break;
	}
}


void RobotTopBoard::_analogWrite(uint8_t codename,int value){
	if(codename==T_TKD4){ // Top Bord TKD4
		uint8_t pin= _codenameToPin(codename);
		analogWrite(pin,value);
	}
}


void RobotTopBoard::_digitalRead(uint8_t codename){
	uint8_t type=_getTypeCode(codename);
	switch(type){
		case TYPE_TOP_TK:
			_digitalReadTopMux(codename);
			break;
		case TYPE_TOP_TKD:
			_digitalReadTopPin(codename);
			break;
	}
}


void RobotTopBoard::_analogRead(uint8_t codename){
	uint8_t type=_getTypeCode(codename);
	switch(type){
		case TYPE_TOP_TK:
			return _analogReadTopMux(codename);
			break;
		case TYPE_TOP_TKD:
			return _analogReadTopPin(codename);
			break;
	}
}


void RobotTopBoard::_readKeybord(){
	int value=analogRead(KEY);
	messageOut.writeByte(COMMAND_ANALOG_READ_KEY_RE);
	messageOut.writeInt(value);
	messageOut.sendData();	
}


void RobotTopBoard::_readKnob(){
	int value=analogRead(POT);
	messageOut.writeByte(COMMAND_READ_POT_RE);
	messageOut.writeInt(value);
	messageOut.sendData();	
}


void RobotTopBoard::_averageReadKeybord(){
	int8_t value=_averageKeyboardRead();
	messageOut.writeByte(COMMAND_AVERAGE_READ_KEY_RE);
	messageOut.writeByte(value);
	messageOut.sendData();	
}


void RobotTopBoard::_playFile(){
	messageIn.readStr(ArStr,1);
	byte checkBusy=messageIn.readByte();
	Serial.print(ArStr); Serial.print(", ");
	Serial.println(checkBusy);
	if(checkBusy == SKEEP_isPlAY){                   // return, if busy
		if(tmrpcm.isPlaying()) return;
	}
	else if(checkBusy == IGNORE_isPlAY){             // ignore isPlAY
		tmrpcm.play(ArStr);
	}
}


void RobotTopBoard::_playFileTime(){
	messageIn.readStr(ArStr,3);
	Tsec=messageIn.readInt();
	byte checkBusy=messageIn.readByte();
	Serial.print(ArStr); Serial.print(", ");
	Serial.print(Tsec); Serial.print(", ");
	Serial.println(checkBusy);

	if(checkBusy == SKEEP_isPlAY){                   // return, if busy
		if(tmrpcm.isPlaying()) return;
	}
	else if(checkBusy == IGNORE_isPlAY){             // ignore isPlAY
		tmrpcm.play(ArStr,Tsec);
	}
}


void RobotTopBoard::_stopPlayback(){

	tmrpcm.stopPlayback();
}


void RobotTopBoard::_disablePlay(){

	tmrpcm.disable();
}


void RobotTopBoard::_pausePlay(){
	
	tmrpcm.pause();
}


void RobotTopBoard::_setVolume(){
	byte vol=messageIn.readByte();
	vol=vol % 8;
	tmrpcm.setVolume(vol);	//0 to 7. Set volume level
}


void RobotTopBoard::_changeVolume(){
	byte vol=messageIn.readByte();  //1(up) or 0(down) to control volume
	vol=vol % 2;
	tmrpcm.volume(vol);	
}


void RobotTopBoard::_playFileLoop(){
	byte set=messageIn.readByte();
	set=set % 2;
	tmrpcm.loop(set);		//0 or 1. Can be changed during playback for full control of looping
}


void RobotTopBoard::_setQuality(){
	byte set=messageIn.readByte();
	set=set % 2;
	tmrpcm.quality(set);		//Set 1 for 2x oversampling
}

void RobotTopBoard::_isPlaying(){
	byte value=tmrpcm.isPlaying();	//returns 1 if music playing, 0 if not
	messageOut.writeByte(COMMAND_IS_PLAYING_RE);
	messageOut.writeByte(value);
	messageOut.sendData();	
}


void RobotTopBoard::_playSoundBuzz(){
	nSound=messageIn.readByte();
	Serial.print("PlayBuzz"); Serial.print(", ");
	Serial.println(nSound);

	PlaySoundBuzz(nSound);
}



RobotTopBoard RobotTop=RobotTopBoard();
