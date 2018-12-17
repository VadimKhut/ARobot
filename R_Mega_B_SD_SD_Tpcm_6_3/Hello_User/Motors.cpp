#include "ArduinoRobot.h"#include "EasyTransfer2.h"extern void keySelection();extern bool F_stop;extern int old_speedML;
extern int old_speedMR;
extern bool F_DebugOutput;
void RobotControl::motorsStop(){	messageOut2.writeByte(COMMAND_MOTORS_STOP);	messageOut2.sendData();#ifdef DEBUG	if (F_DebugOutput) {
		Serial.println(F("==mStop"));	}#endif	old_speedML=0;	old_speedMR=0;}void RobotControl::motorsWrite(int speedLeft,int speedRight){#ifdef DEBUG	if (F_DebugOutput) {
		Serial.print(F("spL="));		Serial.print(speedLeft);		Serial.print(F(" spR="));		Serial.print(speedRight);	}#endif	if((old_speedML == speedLeft) && (old_speedMR == speedRight)){#ifdef DEBUG		if (F_DebugOutput) {
			Serial.println(F(" old==speed"));		}#endif		return;	}#ifdef DEBUG		if (F_DebugOutput) {
			Serial.println();		}#endif	old_speedML=speedLeft;	old_speedMR=speedRight;	messageOut2.writeByte(COMMAND_RUN);	messageOut2.writeInt(speedRight);	messageOut2.writeInt(speedLeft);	messageOut2.sendData();}void RobotControl::motorsWritePct(int speedLeftPct, int speedRightPct){	int16_t speedLeft=255*speedLeftPct;	int16_t speedRight=255*speedRightPct;		motorsWrite(speedLeft,speedRight);}void RobotControl::pointTo(int angle){	int target=angle;	uint8_t speed=120;	int key;	target=target %360;	if(target < 0){		target+=360;	}	int direction=angle; //303#ifdef DEBUG	if (F_DebugOutput) {
		Serial.print(F("poTo="));		Serial.println(angle);	}#endif	while(1){		if(direction > 0){			motorsWrite(-speed, speed);	//right			delay(10);		}		else{			motorsWrite(speed, -speed);	//left			delay(10);		}		int currentAngle=compassRead();#ifdef DEBUG	if (F_DebugOutput) {
		Serial.print(F("CmCarr: "));
		Serial.println(currentAngle);
	}
#endif		int diff=target-currentAngle;		if(diff < -180) 			diff += 360;		else if(diff > 180) 			diff -= 360;		direction=-diff;				if(abs(diff) < 5){			motorsWrite(0,0);			return;		}		getPS2ControlInput();		keySelection();		if(F_stop){			return;		}	}}void RobotControl::turn(int angle){	int originalAngle=compassRead();#ifdef DEBUG	if (F_DebugOutput) {
		Serial.print(F("CmOrg: "));
		Serial.println(originalAngle);
	}
#endif	int target=originalAngle+angle;	pointTo(target);}void RobotControl::moveForward(int speed){	motorsWrite(speed,speed);}void RobotControl::moveBackward(int speed){	motorsWrite(speed,speed);}void RobotControl::turnLeft(int speed){	motorsWrite(speed,255);}void RobotControl::turnRight(int speed){	motorsWrite(255,speed);}/*int RobotControl::getIRrecvResult(){	messageOut.writeByte(COMMAND_GET_IRRECV);	messageOut.sendData();	//delay(10);	while(!messageIn.receiveData());		if(messageIn.readByte()==COMMAND_GET_IRRECV_RE){		return messageIn.readInt();	}	return -1;}*/