#include "ArduinoRobot.h"
#include "Wire.h"
#include "RobotTextManager.h"

extern bool F_DebugOutput;
extern bool F_DebugVlight;
extern bool F_DebugVoltage;
extern bool F_DebugOutput;
extern bool F_UDistance;
extern bool F_DebugWiskersTest;
extern bool F_DebugWiskers;
extern bool F_powerOn;
extern bool F_enableLightAuto;
extern bool F_displ_ErrorVolt;


//**********************************************
uint16_t RobotControl::UDistanceFr=0;
uint16_t RobotControl::UDistanceRv=0;

byte RobotControl::cntCheckWiskerLF=0;
byte RobotControl::cntCheckWiskerRF=0;
byte RobotControl::cntCheckWiskerLR=0;
byte RobotControl::cntCheckWiskerRR=0;

//**********************************************




bool RobotControl::digitalRead(uint8_t port){
	uint8_t type=_getTypeCode(port);
	switch(type){
		case TYPE_TOP_TK:
			return _requestDigitalReadTop(port);
			break;
		case TYPE_TOP_TKD:
			return _requestDigitalReadTop(port);
			break;
		case TYPE_BOTTOM_TKD:
			return _requestDigitalReadBot(port);
			break;
		case TYPE_BOTTOM_SENSE:
			return _requestDigitalReadBot(port);
			break;
	}
}


int RobotControl::analogRead(uint8_t port){
	uint8_t type=_getTypeCode(port);

	switch(type){
		case TYPE_TOP_TK:
			return _requestAnalogReadTop(port);
			break;
		case TYPE_TOP_TKD:
			return _requestAnalogReadTop(port);
			break;
		case TYPE_BOTTOM_TKD:
			return _requestAnalogReadBot(port);
			break;
		case TYPE_BOTTOM_TK:
			return _requestAnalogReadBot(port);
			break;
		case TYPE_BOTTOM_SENSE:
			return _requestAnalogReadBot(port);
			break;
	}
}


void RobotControl::digitalWrite(uint8_t port, bool value){
	uint8_t type=_getTypeCode(port);
	switch(type){
		case TYPE_TOP_TK:
			//Top TKs can't use digitalWrite?
			break;
		case TYPE_TOP_TKD:
			_requestDigitalWriteTop(port, value);
			break;
		case TYPE_BOTTOM_TKD:
			_requestDigitalWriteBot(port, value);
			break;
	}
}
void RobotControl::analogWrite(uint8_t port, uint8_t value){
	if(port==T_TKD4)
	_requestAnalogWriteTop(port, value); // T_TKD4
}


uint8_t RobotControl::_getTypeCode(uint8_t port){
	switch(port){
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
			
		case B_TKD1:
		case B_TKD2:
		case B_TKD3:
		case B_TKD4:
			return TYPE_BOTTOM_TKD;
			break;

		case B_IR0:
		case B_IR1:
		case B_IR2:
		case B_IR3:
		case B_IR4:
		case B_VBAT:
			return TYPE_BOTTOM_TK;
			break;

		case B_SENSE_A:
		case B_SENSE_B:
			return TYPE_BOTTOM_SENSE;
			break;
	}
}


bool RobotControl::_requestDigitalReadTop(uint8_t port){
	messageOut1.writeByte(COMMAND_DIGITAL_READ);
	messageOut1.writeByte(port);//T_TK, T_TKD
	messageOut1.sendData();
	delay(10);
	if(messageIn1.receiveData()){
		//Serial.println("*************");
		uint8_t cmd=messageIn1.readByte();
		//Serial.print("cmd: ");
		//Serial.println(cmd);
		if(!(cmd==COMMAND_DIGITAL_READ_RE))
			return false;
			
		uint8_t dt=messageIn1.readByte();
		return dt;
	}
}


bool RobotControl::_requestDigitalReadBot(uint8_t port){
	messageOut2.writeByte(COMMAND_DIGITAL_READ);
	messageOut2.writeByte(port);//B_TK1 - B_TK4
	messageOut2.sendData();
	delay(10);
	if(messageIn2.receiveData()){
		//Serial.println("*************");
		uint8_t cmd=messageIn2.readByte();
		//Serial.print("cmd: ");
		//Serial.println(cmd);
		if(!(cmd==COMMAND_DIGITAL_READ_RE))
			return false;
			
		uint8_t dt=messageIn2.readByte();
		return dt;
	}
}


int RobotControl::_requestAnalogReadTop(uint8_t port){
	messageOut1.writeByte(COMMAND_ANALOG_READ);
	messageOut1.writeByte(port);//T_TK, T_TKD
	messageOut1.sendData();
	delay(10);
	if(messageIn1.receiveData()){
		uint8_t cmd=messageIn1.readByte();
		//Serial.println("*************");
		//Serial.print("cmd: ");
		//Serial.println(cmd);
		if(!(cmd==COMMAND_ANALOG_READ_RE))
			return false;
			
		uint16_t dt=messageIn1.readInt();
		return dt;
	}
}


//uint16_t RobotControl::_requestAnalogReadBot(uint8_t port){
//	messageOut2.writeByte(COMMAND_ANALOG_READ);
//	messageOut2.writeByte(port);//B_TK1 - B_TK4
//	messageOut2.sendData();
//	delay(10);
//	if(messageIn2.receiveData()){
//		uint8_t cmd=messageIn2.readByte();
//		//Serial.println("*************");
//		//Serial.print("cmd: ");
//		//Serial.println(cmd);
//		if(!(cmd==COMMAND_ANALOG_READ_RE))
//			return false;
//			
//		uint16_t dt=messageIn2.readInt();
//		return dt;
//	}
//}


int RobotControl::_requestAnalogReadBot(uint8_t port){

	messageOut2.writeByte(COMMAND_ANALOG_READ);
	messageOut2.writeByte(port);//B_TK1 - B_TK4
	messageOut2.sendData();
	delay(20);
	while(!messageIn2.receiveData());
	uint8_t cmd=messageIn2.readByte();
	//Serial.println("*************");
	//Serial.print("cmd: ");
	//Serial.println(cmd);
	if(!(cmd == COMMAND_ANALOG_READ_RE))
		return -1;
			
	uint16_t dt=messageIn2.readInt();
	return dt;
}

	
void RobotControl::_requestDigitalWriteTop(uint8_t port, uint8_t value){
	messageOut1.writeByte(COMMAND_DIGITAL_WRITE);
	messageOut1.writeByte(port);//T_TKD...
	messageOut1.writeByte(value);
	messageOut1.sendData();
}


void RobotControl::_requestDigitalWriteBot(uint8_t port, uint8_t value){
	messageOut2.writeByte(COMMAND_DIGITAL_WRITE);
	messageOut2.writeByte(port);//B_TK1 - B_TK4
	messageOut2.writeByte(value);
	messageOut2.sendData();
}


void RobotControl::_requestAnalogWriteTop(uint8_t port, uint8_t value){
	messageOut1.writeByte(COMMAND_ANALOG_WRITE);
	messageOut1.writeByte(port); // T_TKD4 Only
	messageOut1.writeByte(value);
	messageOut1.sendData();
}


void RobotControl::updateIR(){
	messageOut2.writeByte(COMMAND_READ_IR);
	messageOut2.sendData();
	delay(10);
	if(messageIn2.receiveData()){
		if(messageIn2.readByte()==COMMAND_READ_IR_RE){
			for(int i=0; i < 5; i++){
				IRarray[i]=messageIn2.readInt();
			}
		}
	}
}


int RobotControl::knobRead(){
	messageOut1.writeByte(COMMAND_READ_POT);
	//Serial.println("*************");
	//Serial.print("cmd: ");
	//Serial.println(COMMAND_READ_POT);

	messageOut1.sendData();
	delay(10);
	while(!messageIn1.receiveData());
	uint8_t cmd=messageIn1.readByte();
	//Serial.print("cmd: ");
	//Serial.println(cmd);
	if(!(cmd==COMMAND_READ_POT_RE))
		return false;
			
	uint16_t dt=messageIn1.readInt();
	//Serial.print("pot: ");
	//Serial.println(dt);
	return dt;
}


int RobotControl::analogReadKey(){
	messageOut1.writeByte(COMMAND_ANALOG_READ_KEY);
	//Serial.println("*************");
	//Serial.print("cmd: ");
	//Serial.println(COMMAND_ANALOG_READ_KEY);
	messageOut1.sendData();
	delay(10);
	while(!messageIn1.receiveData());
	uint8_t cmd=messageIn1.readByte();
	#ifdef DEBUG
		Serial.print("cmd: ");
		Serial.println(cmd);
	#endif
	if(!(cmd==COMMAND_ANALOG_READ_KEY_RE))
		return false;
	uint16_t dt=messageIn1.readInt();
	#ifdef DEBUG
		Serial.print("key: ");
		Serial.println(dt);
	#endif
	return dt;
}

//int RobotControl::trimRead(){
//	messageOut2.writeByte(COMMAND_READ_TRIM);
//	messageOut2.sendData();
//	delay(10);
//	if(messageIn2.receiveData()){
//		uint8_t cmd=messageIn2.readByte();
//		if(!(cmd==COMMAND_READ_TRIM_RE))
//			return false;
//			
//		uint16_t dt=messageIn2.readInt();
//		return dt;
//	}
//}
//

int RobotControl::trimRead(){
	messageOut2.writeByte(COMMAND_READ_TRIM);
	messageOut2.sendData();
	delay(10);
	while(!messageIn2.receiveData());
	uint8_t cmd=messageIn2.readByte();
	if(!(cmd==COMMAND_READ_TRIM_RE))
		return -1;
			
	uint16_t dt=messageIn2.readInt();
	return dt;
}


uint16_t RobotControl::compassRead(){
	return Compass::getReading();
}

//*******************************************************************
// Ultrasonic
uint16_t RobotControl::GetDistanceFr(){
	uint16_t ret;  
	return ret=ultrasonicFr.Ranging(CM);
}


//*********************************************
//return 0 - < 50 ms   
//return 1 - UDistanceFr <  30
//return 2 - UDistanceFr <  40
//return 3 - UDistanceFr >= 40
//*********************************************
uint8_t RobotControl::UScheckDistanceFr() {
  
	if((millis()-old_USDelayFr) < 50){
	#ifdef DEBUG
		if (F_DebugOutput) {
			if (F_UDistance) {
				Serial.println(F(" <50"));
			}
		}
	#endif
		return 0;
	}

	UDistanceFr=GetDistanceFr();
	old_USDelayFr=millis();

#ifdef DEBUG
	if(F_DebugOutput){
		if (F_UDistance){
			DBGSerial.print(F("  DistF="));
			DBGSerial.println(UDistanceFr, DEC);
		}
	}  
#endif
    
	if(UDistanceFr < 30){		// <30cm
  		return 1;
	}
	else if(UDistanceFr < 40){	// <40cm
		return 2;
	}
	else{
		return 3;				// >= 40
	}  
}  
  
//*******************************************************************
uint16_t RobotControl::GetDistanceRv(){
	uint32_t ret;  
	return ret=ultrasonicRv.Ranging(CM);
}


//*********************************************
//return 0 - < 50 ms   
//return 1 - UDistanceRv <  30
//return 2 - UDistanceRv <  40   
//return 3 - UDistanceRv >= 40
//*********************************************
uint8_t RobotControl::UScheckDistanceRv(){
	uint16_t DistanceRv;


	if((millis()-old_USDelayRv) < 50){
#ifdef DEBUG
		if (F_DebugOutput){
			if (F_UDistance){
				Serial.println(F(" <50"));
			}
		}
#endif
		return 0;
	}

	UDistanceRv=GetDistanceRv();

	old_USDelayRv=millis();

#ifdef DEBUG
	if (F_DebugOutput) {
		if (F_UDistance) {
			DBGSerial.print(F("  DistR="));
			DBGSerial.println(UDistanceRv, DEC);
		}
	}  
#endif
  
	if(UDistanceRv < 30){		// < 30cm
  		return 1;
	}
	else if(UDistanceRv < 40){	// < 40cm
  		return 2;
	}
	else{						// >= 40cm
		return 3;
	}  
}  



//*******************************************************************
void RobotControl::checkLight(){
	uint16_t aread;

	F_enableCheckLight_1s=false;

	if(!F_enableLightAuto)
		return;
	
	aread = ::analogRead(V_LIGHT);

#ifdef DEBUG
	if (F_DebugOutput){
		if (F_DebugVlight) {
			Serial.print(F("V_LIGHT= "));
			Serial.println(aread);
		}
	}  
#endif

	if((aread < 573) && !F_lightON){
		countCheckLight++;
		if(countCheckLight == 2){
			FrontLightOnOff(true);
			countCheckLight=0;
		}
	}
	else if((aread > 573) && F_lightON){
		countCheckLight++;
		if(countCheckLight == 2){
			FrontLightOnOff(false);
			countCheckLight=0;
		}
	}
	else{
		countCheckLight=0;
	}
}


void RobotControl::FrontLightOnOff(bool setLight){

	if(setLight){
		::digitalWrite(LIGHT_ON, HIGH);	// LIGHT ON
		F_lightON=true;
	}
	else if(!setLight){
		::digitalWrite(LIGHT_ON, LOW);	// LIGHT OFF
		F_lightON=false;
	}
}


//*********************************************
//return 0 - > 10 cm   
//return 1 - <=  10cm and < cntCheckWiskerXX
//return 2 - <=  10cm and == cntCheckWiskerXX
//*********************************************
uint8_t RobotControl::getWiskerLF(){
	uint16_t aread;

	aread = ::analogRead(WISKER_LF);
#ifdef DEBUG
	if (F_DebugOutput) {
		if (F_DebugWiskersTest) {
			Serial.print(F(" W_LF= "));
			Serial.println(aread);
		}
	}
#endif
	if(aread > 400){
		cntCheckWiskerLF++;
		if(cntCheckWiskerLF == MAX_CHECK_WISKERS){
			cntCheckWiskerLF=0;
		#ifdef DEBUG
			if (F_DebugOutput) {
				if (F_DebugWiskers){
					Serial.print(F(" W_LF= "));
					Serial.print(aread);
					Serial.println(F(" OK>>"));
				}
			}  
		#endif
			return 2;
		}
		else
			return 1;
	}
	else{
		cntCheckWiskerLF=0;
		return 0;
	}
}


uint8_t RobotControl::getWiskerRF(){
	uint16_t aread;

	aread = ::analogRead(WISKER_RF);
#ifdef DEBUG
	if (F_DebugOutput) {
		if (F_DebugWiskersTest){
			Serial.print(F(" W_RF= "));
			Serial.println(aread);
		}
	}
#endif
	if(aread > 400){
		cntCheckWiskerRF++;
		if(cntCheckWiskerRF == MAX_CHECK_WISKERS){
			cntCheckWiskerRF=0;
	#ifdef DEBUG
			if (F_DebugOutput) {
				if (F_DebugWiskers) {
					Serial.print(F(" W_RF= "));
					Serial.print(aread);
					Serial.println(F(" OK>>"));
				}
			}  
	#endif
			return 2;
		}
		else
			return 1;
	}
	else{
		cntCheckWiskerRF=0;
		return 0;
	}
}


uint8_t RobotControl::getWiskerLR(){
	uint16_t aread;

	aread = ::analogRead(WISKER_LR);
#ifdef DEBUG
	if (F_DebugOutput){
		if (F_DebugWiskersTest){
			Serial.print(F(" W_LR= "));
			Serial.println(aread);
		}
	}
#endif
	if(aread > 400){
		cntCheckWiskerLR++;
		if(cntCheckWiskerLR == MAX_CHECK_WISKERS){
			cntCheckWiskerLR=0;
	#ifdef DEBUG
			if (F_DebugOutput) {
				if (F_DebugWiskers) {
					Serial.print(F(" W_LR= "));
					Serial.print(aread);
					Serial.println(F(" OK>>"));
				}
			}  
	#endif
			return 2;
		}
		else
			return 1;
	}
	else{
		cntCheckWiskerLR=0;
		return 0;
	}
}


uint8_t RobotControl::getWiskerRR(){
	uint16_t aread;

	aread = ::analogRead(WISKER_RR);
#ifdef DEBUG
	if (F_DebugOutput){
		if (F_DebugWiskersTest) {
			Serial.print(F(" W_RR= "));
			Serial.println(aread);
		}
	}
#endif
	if(aread > 400){
		cntCheckWiskerRR++;
		if(cntCheckWiskerRR == MAX_CHECK_WISKERS){
			cntCheckWiskerRR=0;
	#ifdef DEBUG
			if (F_DebugOutput){
				if (F_DebugWiskers) {
					Serial.print(F(" W_RR= "));
					Serial.println(aread);
					Serial.println(F(" OK>>"));
				}
			}  
	#endif
			return 2;
		}
		else
			return 1;
	}
	else{
		cntCheckWiskerRR=0;
		return 0;
	}
}



void RobotControl::checkVoltage(){
	int VoltA;
	uint16_t Volt;
	float VoltFl;

	F_enableCheckVBat_1s=false;

	VoltA=analogRead(B_VBAT);
	if(VoltA == -1)
		return;

	Volt=((uint32_t)VoltA*1359)/1000;
	if(Volt >= 1390) // VoltA=1024
		return;

	//Serial.print(F("Va="));
	//Serial.print(VoltA, DEC);
	//Serial.print(F(",  V*10="));
	//Serial.print(Volt, DEC);

	VoltFl=(float)Volt*0.01;

	//Serial.print(F(",  VBat="));
	//Serial.print(VoltFl,2);
	//Serial.println(F("V"));


	if(textManager.old_valueFl != VoltFl){
		Robot.setTextSize(1);
		textManager.debugPrintYX(VoltFl,2,90);
		Robot.print(F("V"));
	}

#ifdef DEBUG
	if (F_DebugOutput){
		if (F_DebugVlight) {
			Serial.print(F("Va="));
			Serial.print(VoltA, DEC);
			Serial.print(F(",  VBat="));
			Serial.print(VoltFl,2);
			Serial.println(F("V"));
		}
	}
#endif

	if((Volt < TURN_OFF_VBAT) || (Volt > MAX_VBAT)){
		Robot.fill(WHITE);
		Robot.stroke(WHITE);
		Robot.rect(0,40,128,40); // erase the previous text
		Robot.stroke(BLACK);
		textManager.writeText_P(PSTR("Low Voltage,\n turn OFF the Robot"),5,1);
		F_displ_ErrorVolt=true;
		playSoundBuzz(3);
	#ifdef DEBUG
		Serial.println(F("Low Voltage"));
			Serial.print(F("VBat="));
			Serial.print(Volt, DEC);
			Serial.print(F("V"));
			Serial.print(F(",  Cn="));
			Serial.println(countVBat, DEC);
	#endif

		if(countVBat > MAX_COUNT_VBAT){
			F_powerOn=false;
			playFileDisable();
			Robot.leds_Stop();
			countVBat=0;
			Robot.fill(WHITE);
			Robot.stroke(WHITE);
			Robot.rect(0,40,128,40); // erase the previous text
			Robot.stroke(BLACK);
			textManager.writeText_P(PSTR("Low Voltage,\n turn OFF the Robot"),5,1);
			textManager.writeText_P(PSTR("Voltage Critical Error\n Shutdown!"),7,1);
			for(int i=0; i < 5; i++){
				playSoundBuzz(3);
				delay(10);
			}
			waitContinueAnyKey_T();
		}
		else{
			countVBat++;
		}
	}
	else if((Volt > TURN_OFF_VBAT) && (Volt < MAX_VBAT)){
		countVBat=0;
	}
}


