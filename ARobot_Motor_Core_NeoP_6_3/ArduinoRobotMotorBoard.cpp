#include "ArduinoRobotMotorBoard.h"
#include "EasyTransfer2.h"
#include "Multiplexer.h"
#include "LineFollow.h"


RobotMotorBoard::RobotMotorBoard(){
	//LineFollow::LineFollow();
	_style=0;
}


void RobotMotorBoard::begin(){
	//initialze communication
	Serial.begin(9600);
	Serial1.begin(9600);
	messageIn.begin(&Serial1);
	messageOut.begin(&Serial1);

	//init MUX
	uint8_t MuxPins[]={MUXA,MUXB,MUXC};
	this->IRs.begin(MuxPins,MUX_IN,3);
	pinMode(MUXI,INPUT);
	digitalWrite(MUXI,LOW);
	
	randomSeed(analogRead(0));

	isPaused=false;
}


void RobotMotorBoard::process(){

	switch(_style){
		case RAINBOW:
			runRainbow();
			break;
		case CYLON_UP:
			runCylon();
			break;
		case CYLON_DOUN:
			runCylon();
			break;
		case CYLON_UP_DOUN:
			runCylon();
			break;
		case CASCADE_UP:
			runCascade();
			break;
		case CASCADE_DOUN:
			runCascade();
			break;
		case CASCADE_UP_DOUN:
			runCascade();
			break;
		case COLOR_WIPE_W_UP:
		case COLOR_WIPE_W_DOUN:
		case COLOR_WIPE_W_UP_DOUN:
			runColorWipe_W();
			break;
		case COLOR_WIPE_R_UP:
		case COLOR_WIPE_R_DOUN:
		case COLOR_WIPE_R_UP_DOUN:
			runColorWipe_R();
			break;
		case COLOR_WIPE_G_UP:
		case COLOR_WIPE_G_DOUN:
		case COLOR_WIPE_G_UP_DOUN:
			runColorWipe_G();
			break;
		case COLOR_WIPE_B_UP:
		case COLOR_WIPE_B_DOUN:
		case COLOR_WIPE_B_UP_DOUN:
			runColorWipe_B();
			break;
		case COLOR_WIPE_RGBW_UP:
		case COLOR_WIPE_RGBW_DOUN:
		case COLOR_WIPE_RGBW_UP_DOUN:
			runColorWipe_RGBW();
			break;
		case THEATRE_CHASE_W_UP:
		case THEATRE_CHASE_W_DOUN:
		case THEATRE_CHASE_W_UP_DOUN:
			runTheaterChase_W();
			break;
		case THEATRE_CHASE_R_UP:
		case THEATRE_CHASE_R_DOUN:
		case THEATRE_CHASE_R_UP_DOUN:
			runTheaterChase_R();
			break;
		case THEATRE_CHASE_G_UP:
		case THEATRE_CHASE_G_DOUN:
		case THEATRE_CHASE_G_UP_DOUN:
			runTheaterChase_G();
			break;
		case THEATRE_CHASE_B_UP:
		case THEATRE_CHASE_B_DOUN:
		case THEATRE_CHASE_B_UP_DOUN:
			runTheaterChase_B();
			break;
		case THEATRE_CHASE_RGBW_UP:
		case THEATRE_CHASE_RGBW_DOUN:
		case THEATRE_CHASE_RGBW_UP_DOUN:
			runTheaterChase_RGBW();
			break;
		case RAINBOW_1:
			runRainbow1();
			break;
		case RAINBOW_CYCLE:
			runRainbowCycle();
			break;
		case THEATRE_CHASE_RAINBOW_UP:
		case THEATRE_CHASE_RAINBOW_DOUN:
		case THEATRE_CHASE_RAINBOW_UP_DOUN:
			runTheaterChaseRainbow();
			break;
	}

	//skip process if the mode is paused
	if(isPaused)
		return;
	
	if(mode==MODE_SIMPLE){
		//Serial.println("s");
		//do nothing. Simple mode is just about getting commands
	}
	else if(mode==MODE_LINE_FOLLOW){
		//do line following
		LineFollow::runLineFollow();
	}
	checkSense();
}




void RobotMotorBoard::parseCommand(){
	uint8_t modeName;
	uint8_t codename;
	int value;
	int speedL;
	int speedR;

	if(this->messageIn.receiveData()){
		//Serial.println("data received");
		uint8_t command=messageIn.readByte();
		//Serial.println(command);
		switch(command){
			case COMMAND_SWITCH_MODE:
				modeName=messageIn.readByte();
				setMode(modeName);
				break;
			case COMMAND_RUN:
				//in follow line mode, the motor does not follow commands
				if(mode == MODE_LINE_FOLLOW)
					break;
				speedL=messageIn.readInt();
				speedR=messageIn.readInt();
				motorsWrite(speedL,speedR);
				break;
			case COMMAND_MOTORS_STOP:
				motorsStop();
				break;
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
			case COMMAND_READ_IR:
				_readIR();
				break;
			case COMMAND_READ_TRIM:
				_readTrim();
				break;
			case COMMAND_PAUSE_MODE:
				pauseMode(messageIn.readByte());//onOff state
				break;
			case COMMAND_LINE_FOLLOW_CONFIG:
				_config();
				break;
			case COMMAND_LEDS_SHOW:
				_leds_show();
				break;
			case COMMAND_LEDS_IS_SHOW:
				_leds_Is_Show();
				break;
			case COMMAND_LEDS_STOP:
				_leds_stop();
				break;
			case COMMAND_PAUSE_SHOW_LEDS:
				_leds_pause_show();
				break;
		} //END switch()
	}
	//delay(5);
}





uint8_t RobotMotorBoard::parseCodename(uint8_t codename){

	switch(codename){
		case B_TKD1:
			return TK1;
		case B_TKD2:
			return TK2;
		case B_TKD3:
			return TK3;
		case B_TKD4:
			return TK4;
	}
}


uint8_t RobotMotorBoard::codenameToAPin(uint8_t codename){

	switch(codename){
		case B_TKD1:
			return A0;
		case B_TKD2:
			return A1;
		case B_TKD3:
			return A6;
		case B_TKD4:
			return A11;
		case B_SENSE_A:
			return A4;
		case B_SENSE_B:
			return A5;
	}
}


void RobotMotorBoard::setMode(uint8_t mode){

	//if(mode==MODE_LINE_FOLLOW){
	//	LineFollow::calibIRs();
	//}
	/*if(mode==SET_MOTOR_ADJUSTMENT){
			save_motor_adjustment_to_EEPROM();
		}
	*/
	/*if(mode==MODE_IR_CONTROL){
		beginIRReceiver();
	}*/
	this->mode=mode;
	//stopCurrentActions();//If line following, this should stop the motors
}


void RobotMotorBoard::stopCurrentActions(){

	motorsStop();
	//motorsWrite(0,0);
}


void RobotMotorBoard::pauseMode(bool onOff){
	
	if(onOff){
		isPaused=true;
	}
	else{
		isPaused=false;
	}
	stopCurrentActions();
}



void RobotMotorBoard::motorsWrite(int speedL, int speedR){

	/*Serial.print(speedL);
	Serial.print(" ");
	Serial.println(speedR);*/
	//motor adjustment, using percentage


	if(speedL == 0 && speedR == 0)
		F_stopMot=true;
	else
		F_stopMot=false;

	_refreshMotorAdjustment();

	if(motorAdjustment < 0){
		speedR*=(1+motorAdjustment);
	}
	else{
		speedL*=(1-motorAdjustment);
	}
	
	if(speedL > 0){
		analogWrite(IN_A1,speedL);
		analogWrite(IN_A2,0);
	}
	else{
		analogWrite(IN_A1,0);
		analogWrite(IN_A2,-speedL);
	}
	
	if(speedR > 0){
		analogWrite(IN_B1,speedR);
		analogWrite(IN_B2,0);
	}
	else{
		analogWrite(IN_B1,0);
		analogWrite(IN_B2,-speedR);
	}
}


void RobotMotorBoard::motorsWritePct(int speedLpct, int speedRpct){

	//speedLpct, speedRpct ranges from -100 to 100
	motorsWrite(speedLpct*2.55,speedRpct*2.55);
}


void RobotMotorBoard::motorsStop(){

	analogWrite(IN_A1,255);
	analogWrite(IN_A2,255);
	analogWrite(IN_B1,255);
	analogWrite(IN_B2,255);
	F_stopMot=true;
}


void RobotMotorBoard::checkSense(){
	uint16_t senseA;
	uint16_t senseB;


	if(F_stopMot && !F_sense){
		return;
	}

	senseA=analogRead(SENSE_A);
	senseB=analogRead(SENSE_B);

//#ifdef DEBUG
//	Serial.print(F("SenA="));
//	Serial.print(senseA, DEC);
//	Serial.print(F("  "));
//	Serial.print(F("SenB="));
//	Serial.println(senseB, DEC);
//#endif



	if(((senseA > MAX_FOR_SENSE) || (senseB > MAX_FOR_SENSE)) && !F_sense){

		F_sense=true;
		senseTime_old=millis();// start timer 
		//Serial.println(F("-FSt"));
	}
	else if ((senseA <= MAX_FOR_SENSE) && (senseB <= MAX_FOR_SENSE)){
		F_sense=false;
		//Serial.println(F("-FSf"));
	}

	if(((millis()-senseTime_old) > MAX_TIME_SENSE) && F_sense){
		motorsStop();
		F_sense=false;
		//Serial.println(F("-MotSt"));
	} 
}


//Input and Output ports
void RobotMotorBoard::_digitalWrite(uint8_t codename,bool value){
	uint8_t pin=parseCodename(codename);
	digitalWrite(pin,value);
}


void RobotMotorBoard::_analogWrite(uint8_t codename,int value){
	//There's no PWM available on motor board
}


void RobotMotorBoard::_digitalRead(uint8_t codename){
	uint8_t pin=parseCodename(codename);
	bool value=digitalRead(pin);
	messageOut.writeByte(COMMAND_DIGITAL_READ_RE);
//	messageOut.writeByte(codename);
	messageOut.writeByte(value);
	messageOut.sendData();
}


void RobotMotorBoard::_analogRead(uint8_t codename){ // ***Vad
	uint8_t type=_getTypeCode(codename);
	switch(type){
		case TYPE_B_TK:
			return _analogReadBotMux(codename);
			break;
		case TYPE_B_TKD:
			return _analogReadBotPin(codename);
			break;
		case TYPE_B_SENSE:
			return _analogReadBotPin(codename);
			break;
	}
}


void RobotMotorBoard::_analogReadBotPin(uint8_t codename){ // ***Vad
	uint8_t pin=codenameToAPin(codename);
	int value=analogRead(pin);
	messageOut.writeByte(COMMAND_ANALOG_READ_RE);
//	messageOut.writeByte(codename);
	messageOut.writeInt(value);
	messageOut.sendData();
}


void RobotMotorBoard::_analogReadBotMux(uint8_t codename){ // ***Vad
	uint8_t num=_portToBotMux(codename);
	int value=IRs.getAnalogValueAt(num);
	messageOut.writeByte(COMMAND_ANALOG_READ_RE);
	messageOut.writeInt(value);
	messageOut.sendData();
}


uint8_t RobotMotorBoard::_getTypeCode(uint8_t codename){ // ***Vad
	switch(codename){
		case B_IR0:
		case B_IR1:
		case B_IR2:
		case B_IR3:
		case B_IR4:
		case B_VBAT:
			return TYPE_B_TK;
			break;
			
		case B_TKD1:
		case B_TKD2:
		case B_TKD3:
		case B_TKD4:
			return TYPE_B_TKD;
			break;

		case B_SENSE_A:
		case B_SENSE_B:
			return TYPE_B_SENSE;
			break;
	}
}


uint8_t RobotMotorBoard::_portToBotMux(uint8_t codename){ // ***Vad
	switch(codename){
		case B_IR0:
			return 0;
		case B_IR1:
			return 1;
		case B_IR2:
			return 2;
		case B_IR3:
			return 3;
		case B_IR4:
			return 4;
		case B_VBAT:
			return 5;
	}
}


int RobotMotorBoard::IRread(uint8_t num){

	IRs.selectPin(num); //To make consistant with the pins labeled on the board
	return IRs.getAnalogValue();
}


void RobotMotorBoard::_readIR(){

	//Serial.println("readIR");
	int value;
	messageOut.writeByte(COMMAND_READ_IR_RE);
	for(int i=0; i < 5; i++){
		value=IRread(i);
		messageOut.writeInt(value);
	}
	messageOut.sendData();
}


void RobotMotorBoard::_readTrim(){

	int value=analogRead(TRIM);
	messageOut.writeByte(COMMAND_READ_TRIM_RE);
	messageOut.writeInt(value);
	messageOut.sendData();	
}


void RobotMotorBoard::_refreshMotorAdjustment(){
	motorAdjustment=map(analogRead(TRIM),0,1023,-30,30)/100.0;
}


void RobotMotorBoard::reportActionDone(){

	setMode(MODE_SIMPLE);
	messageOut.writeByte(COMMAND_ACTION_DONE);
	messageOut.sendData();
}


void RobotMotorBoard::_config(){

	uint8_t KP=messageIn.readByte();	//KP
	uint8_t KD=messageIn.readByte();	//KD
	uint8_t robotSpeed=messageIn.readByte();	//robotSpeed
	uint8_t intergrationTime=messageIn.readByte();	//IntegrationTime
	LineFollow::config(KP,KD,robotSpeed,intergrationTime);
}


void RobotMotorBoard::_leds_show(){

	uint8_t style=messageIn.readByte();
	uint8_t ncycleInStyle=messageIn.readByte();
	uint8_t ncycleAllStyles=messageIn.readByte();
	uint16_t wait=messageIn.readInt();

#ifdef DEBUG
	Serial.print(F("leds_show("));
	Serial.print(style);
	Serial.print(F(","));
	Serial.print(ncycleInStyle);
	Serial.print(F(","));
	Serial.print(ncycleAllStyles);
	Serial.print(F(","));
	Serial.print(wait);
	Serial.println(F(")"));
#endif

	leds_show(style,ncycleInStyle,ncycleAllStyles,wait);
}


void RobotMotorBoard::leds_show(uint8_t style, uint8_t ncycleInStyle, uint8_t ncycleAllStyles, uint16_t wait){

	_wait=wait;
	_wait_s=wait;
	if(ncycleInStyle == 0)
		ncycleInStyle=1;
	_ncycle=ncycleInStyle;
	_ncycle_s=ncycleInStyle;
	_ncycleAllStyles=ncycleAllStyles;
	_ncycleAllStyleRandom=0;
	F_pause=false;
	F_stop=false;

	//  set flag for SHOW_CONTINUE in style
	if(ncycleInStyle == SHOW_CONTINUE)
		F_Show_cont=true;
	else 
		F_Show_cont=false;

	//  set flag for SHOW_CONTINUE for all styles
	if(ncycleAllStyles == SHOW_CONTINUE)
		F_Show_contAllStyles=true;
	else 
		F_Show_contAllStyles=false;

	//  set flag for *_ALL_STYLES_*
	if((style == SHOW_ALL_STYLES_UP) || 
		(style == SHOW_ALL_STYLES_DOUN) ||
		(style == SHOW_ALL_STYLES_RANDOM)){

		F_showAllStyles=true;
	}
	else{
		F_showAllStyles=false;
	}

	//  set MAX ncycleInStyle=5 if F_showAllStyles=true
	if(ncycleInStyle > 5 && F_showAllStyles){
		_ncycle=5;
		_ncycle_s=5;
	}

	if(style == SHOW_ALL_STYLES_UP){
		_style = 1;
		F_AllStyle_Up=true;
		F_AllStyleRandom=false;
	}
	else if(style == SHOW_ALL_STYLES_DOUN){
		_style = MAX_STYLES;
		F_AllStyle_Up=false;
		F_AllStyleRandom=false;
	}
	else if(style == SHOW_ALL_STYLES_RANDOM){
		_style=_getStyleRandom(MAX_STYLES);
		F_AllStyleRandom=true;

	}
	else{
		_style=style;
	}

	_initFlagsForLedsShow();
	clearLEDs();
	leds.show();

#ifdef DEBUG
	Serial.print(F("F_showAllStyles= "));
	Serial.print(F_showAllStyles);
	Serial.print(F("; F_Show_cont= "));
	Serial.println(F_Show_cont);
	Serial.print(F("F_AllStyle_Up= "));
	Serial.print(F_AllStyle_Up);
	Serial.print(F("; F_AllStyleRandom= "));
	Serial.println(F_AllStyleRandom);
	Serial.print(F("_style= "));
	Serial.println(_style);
#endif
}


void RobotMotorBoard::_initFlagsForLedsShow(){

	F_StartDisplay=true;
	F_show_up=true;
	F_clearLEDs=false;
	_color=C_RED;
	F_color_Wipe_RGBW=false;
	F_theaterChase_RGBW=false;
	F_show_Up_Down=false;
	F_stop=false;
	_i_pos=0;
	_j_pos=0;

	// set flag for *_RGBW_*
	switch(_style){ 
		case COLOR_WIPE_RGBW_UP:
		case COLOR_WIPE_RGBW_DOUN:
		case COLOR_WIPE_RGBW_UP_DOUN:
			F_color_Wipe_RGBW=true;
			break;
		case THEATRE_CHASE_RGBW_UP:
		case THEATRE_CHASE_RGBW_DOUN:
		case THEATRE_CHASE_RGBW_UP_DOUN:
			F_theaterChase_RGBW=true;
			break;
	}

	switch(_style){
		case CYLON_DOUN:
			F_show_up=false;
			_i_pos=LED_COUNT-2;
			break;
		case CASCADE_DOUN:

		case COLOR_WIPE_W_DOUN:
		case COLOR_WIPE_R_DOUN:
		case COLOR_WIPE_G_DOUN:
		case COLOR_WIPE_B_DOUN:
		case COLOR_WIPE_RGBW_DOUN:

		case THEATRE_CHASE_W_DOUN:
		case THEATRE_CHASE_R_DOUN:
		case THEATRE_CHASE_G_DOUN:
		case THEATRE_CHASE_B_DOUN:
		case THEATRE_CHASE_RGBW_DOUN:

		case THEATRE_CHASE_RAINBOW_DOUN:
			F_show_up=false;
			_i_pos=LED_COUNT-1;
			break;

		case COLOR_WIPE_W_UP_DOUN:
		case COLOR_WIPE_R_UP_DOUN:
		case COLOR_WIPE_G_UP_DOUN:
		case COLOR_WIPE_B_UP_DOUN:
		case COLOR_WIPE_RGBW_UP_DOUN:

		case THEATRE_CHASE_W_UP_DOUN:
		case THEATRE_CHASE_R_UP_DOUN:
		case THEATRE_CHASE_G_UP_DOUN:
		case THEATRE_CHASE_B_UP_DOUN:
		case THEATRE_CHASE_RGBW_UP_DOUN:

		case THEATRE_CHASE_RAINBOW_UP_DOUN:
			F_show_Up_Down=true;
			break;
	}
}


void RobotMotorBoard::_initForShowAll(){

	_ncycle = _ncycle_s;
	_initFlagsForLedsShow();

	switch(_style){ 
		case RAINBOW_CYCLE:
		case RAINBOW_1:
			if(_wait > 50)
				_wait = 20;
			break;
		case THEATRE_CHASE_RAINBOW_UP:
		case THEATRE_CHASE_RAINBOW_DOUN:
		case THEATRE_CHASE_RAINBOW_UP_DOUN:
			if((_wait > 80) || (_wait < 50))
				_wait = 51;
			break;
		default:
			_wait = _wait_s;
	}

#ifdef DEBUG
	Serial.print(F("*** _style= "));Serial.println(_style);
	Serial.print(F("_ncycle= "));Serial.println(_ncycle);
	Serial.print(F("_ncycleStyle= "));Serial.println(_ncycleAllStyles);
	Serial.print(F("_i_pos= "));Serial.println(_i_pos);
	Serial.print(F("_j_pos= "));Serial.println(_j_pos);
	Serial.print(F("F_show_up= "));Serial.println(F_show_up);
	Serial.print(F("_wait= "));Serial.println(_wait);
#endif
}


void RobotMotorBoard::_leds_Is_Show(){

	messageOut.writeByte(COMMAND_LEDS_IS_SHOW_RE);
	messageOut.writeByte(_style);
	messageOut.sendData();	
}


void RobotMotorBoard::_leds_stop(){

	clearLEDs();
	leds.show();
	_style=0;
	F_stop=true;
}


void RobotMotorBoard::_leds_pause(){

	clearLEDs();
	leds.show();
	_style=0;
}


void RobotMotorBoard::_leds_pause_show(){

	if(!F_stop){

		F_pause=!F_pause;

		if(F_pause){
			_style_s=_style;
			_leds_pause();
		}
		else{
			_style=_style_s;
			_initFlagsForLedsShow();
		}
	}
}


uint8_t RobotMotorBoard:: _getStyleRandom(uint8_t maxNumStyles){
	uint32_t randNumber;

	randNumber = random(1,maxNumStyles);
	return (uint8_t)randNumber;
}




RobotMotorBoard RobotMotor=RobotMotorBoard();
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
