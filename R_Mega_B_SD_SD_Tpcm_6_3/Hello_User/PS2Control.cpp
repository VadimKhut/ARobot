/*
Joystick
---------
===================================================================================================

LX/RX=0   - Left
LX/RX=128 - Centre
LX/RX=255 - Right

LY/RY=0   - Up
LY/RY=127 - Centre
LY/RY=255 - Down

----------------------------------------------------------------------------------------------------

		 0						128							255
		
	0	128	  255		-128	 0		128			-255	  0		-255

		255						-128						-255

===================================================================================================
*/
#include "PS2X_lib.h"
#include "ArduinoRobot.h"
#include "scripts_Hello_User.h"

//--------------------------------------------------------------------
#define   PS2_DAT_p		6		// INPUT*	green
#define   PS2_CMD_p		7		// OUTPUT*	blue
#define   PS2_SEL_p		8		// OUTPUT*	purple
#define   PS2_CLK_p		9		// OUTPUT*	gray

#define   ONE_JOYSTICK	0
#define   TWO_JOYSTICK	1

#define	MAX_PS2_ERROR_CNT	5	// How many times through the loop will we go before shutting off robot?

extern void printARobotOff();
extern void turnARobotOff();
extern void getFaleNameFromScriptPGM(const char* const* FaleName_script_P, uint8_t nStr, const char* FaleNameExt_scripts_P);
extern void PrintLxLyRxRy(int Lx, int Ly, int Rx, int Ry);
extern void resetFlags();
extern void resetFlags1();
extern void SetChoiceMusic(uint8_t track);

extern bool F_powerOn;
extern bool F_stop;
extern bool F_manual;
extern bool F_manual1;
extern int speedML;
extern int speedMR;
extern byte stateMove;
extern bool F_DebugPS2Input;
extern bool F_DebugOutput;
extern bool F_LxLyRxRy;
extern bool F_forvard;
extern bool F_reverce;
extern bool F_TurnToOpening;
extern int old_speedML;
extern int old_speedMR;
extern bool F_contFromStop;
extern bool F_ShowDebugPrompt;
extern bool F_blockJoystick;
extern bool F_blockContinue;
extern bool F_skeepWaitAnyTime;
extern bool F_DanceBot;
extern bool F_ChoiceMusic;
extern uint16_t curSeq;
extern int8_t PS_PAD_key;
extern int calSpeedY;
extern uint8_t nMusic;
extern char fileName[13];
extern uint8_t nMusicRedy;
extern bool F_enableSaund;
extern bool F_enableLightAuto;
extern bool F_displSmilesEn;

//Define Variables:
bool ifForvard();
bool ifReverce();
byte mode;
byte PS2ErrorCnt;
int Lx;
int Ly;
int Rx;
int Ry;


 //-------------------------------------------------------------------------------
 //The function to initialize the input controller, which in this case is the 
 //PS2 controller process any commands.
 //-------------------------------------------------------------------------------
int RobotControl::PS2ControlInit(void){
	int error = 0;
	byte type = 0;

	// Setup gamepad (clock, command, attention, data) pins
	error = ps2x.config_gamepad(PS2_CLK_p, PS2_CMD_p, PS2_SEL_p, PS2_DAT_p);

#ifdef DEBUG
	Serial.print(F("PS2 : "));
	Serial.println(error, DEC);
#endif

	if(error){
	#ifdef DEBUG
		Serial.println(F("PS2 check ERROR!!"));
	#endif   
		//MSound (2, 40, 2500, 40, 2500);			//[40\5000,40\5000]
	}   

	type = ps2x.readType(); 
	switch(type) {
		case 0:
	#ifdef DEBUG
			Serial.println(F("Unknown Controller type"));
	#endif
			break;
		case 1:
	#ifdef DEBUG
			Serial.println(F("DualShock Controller Found"));
	#endif
		break;
	}

	return error;
}


//-------------------------------------------------------------------------------
// Function to read inputs from the PS2 and then process any commands.
//-------------------------------------------------------------------------------
void RobotControl::getPS2ControlInput(void){
	int resYX;

	PS_PAD_key= -1;

	// Then try to receive a packet of information from the PS2.
	ps2x.read_gamepad();						// read controller and set large motor to spin at 'vibrate' speed

	if ((ps2x.Analog(1) & 0xf0) == 0x70){

	#ifdef DEBUG
		if (F_DebugOutput){
			if (F_DebugPS2Input) {
				Serial.print(F("PS2 Input: "));
				Serial.print(ps2x.ButtonDataByte(), HEX);
				Serial.print(F(":"));
				Serial.print(ps2x.Analog(PSS_LX), DEC);
				Serial.print(F(" "));
				Serial.print(ps2x.Analog(PSS_LY), DEC);
				Serial.print(F(" "));
				Serial.print(ps2x.Analog(PSS_RX), DEC);
				Serial.print(F(" "));
				Serial.println(ps2x.Analog(PSS_RY), DEC);
			}
		}  
	#endif
		PS2ErrorCnt = 0;							// clear error counter

		if (ps2x.ButtonPressed(PSB_START)){			// Start Button Test 
			playSoundBuzz(7);
			if(stateMove == TURN_TO_OPENING){		// or RUN_DANCE
				F_stop=true;
				motorsStop();
				//old_speedML=0; old_speedMR=0;
				FrontLightOnOff(false);
				if(nMusicRedy == 255)
					pausePlayFile();
				//leds_Stop();
				leds_Pause_Show();
				resetFlags();
				resetFlags1();
				stateMove=STOP_STATE;
			}
			else if(stateMove == STOP_STATE){
				F_stop=false;
				F_contFromStop = true;
				if(nMusicRedy == 255)
					pausePlayFile();
				leds_Pause_Show();
				stateMove=TURN_TO_OPENING; // or RUN_DANCE
			}
			else if(stateMove == ANY_STATE){
				PS_PAD_key=BUTTON_MIDDLE;
				if(!F_blockContinue){
					F_stop=!F_stop;
					if(nMusicRedy == 255)
						pausePlayFile();
					leds_Pause_Show();
					F_contFromStop=true;
					if(F_stop){
						motorsStop();
						//leds_Stop();
					}
				}
			}
		}

		if (ps2x.ButtonPressed(PSB_PAD_UP)){
			playSoundBuzz(7);
			PS_PAD_key=BUTTON_UP;
		}

		if (ps2x.ButtonPressed(PSB_PAD_DOWN)){
			playSoundBuzz(7);
			PS_PAD_key=BUTTON_DOWN;
		}

		if (ps2x.ButtonPressed(PSB_PAD_RIGHT)){
			playSoundBuzz(7);
			if(stateMove == POWER_STATE_OFF){
				curSeq=0;
				F_contFromStop=false;
				stateMove=TURN_TO_OPENING; // or RUN_DANCE
			}
			else if(stateMove == ANY_STATE){
				PS_PAD_key=BUTTON_RIGHT;
				F_skeepWaitAnyTime=true;
			}
			F_powerOn=true;
		}

		if (ps2x.ButtonPressed(PSB_PAD_LEFT)){
			playSoundBuzz(7);
			if(stateMove == STOP_STATE){
				Robot.leds_Stop();
				F_powerOn=false;
				F_stop=false;
				stateMove=POWER_STATE_OFF;
			}
			else if(stateMove == POWER_STATE_OFF){
				Robot.stopPlayFile();
				Robot.playFileDisable();
				F_powerOn=false;
				F_stop=false;
				stateMove=RETURN_STATE;
			}
			else if(stateMove == ERROR_STATE){
				F_powerOn=false;
				F_stop=false;
				stateMove=RETURN_STATE;
			}
			else if(stateMove == ANY_STATE){
				PS_PAD_key=BUTTON_LEFT;
				if(F_stop){
					stopPlayFile();
					playFileDisable();
					Robot.leds_Stop();
					F_blockContinue=false;
					F_powerOn=false;
					F_stop=false;
				}
			}
			F_ShowDebugPrompt=true;
		}
		if(stateMove == ANY_STATE){						// speed for Wheel Calibration

			calSpeedY = -(ps2x.Analog(PSS_LY)-127);

			if(calSpeedY > 5)
				calSpeedY=map(calSpeedY,6,127,70,255);
			else if(calSpeedY < -5)
				calSpeedY=map(calSpeedY,-6,-128,-70,-255);

			PrintLxLyRxRy(0,calSpeedY,0,0);
		}


		if (ps2x.ButtonPressed(PSB_CROSS)) {			// X - Cross Button Test
			playSoundBuzz(7);								// [50\4000]
			F_enableSaund=!F_enableSaund;
			if(F_enableSaund){
				Serial.println(F("==SAUND ENABLE"));
			}
			else if(!F_enableSaund){
				Serial.println(F("==SAUND DESABLE"));
			}
		}

		if (ps2x.ButtonPressed(PSB_SQUARE)) {			// Square Button Test 
			playSoundBuzz(7);
			F_displSmilesEn=!F_displSmilesEn;
			if(F_displSmilesEn){
				Serial.println(F("==Display Smile ENABLE"));
			}
			else if(!F_displSmilesEn){
				FrontLightOnOff(false);
				Serial.println(F("==Display Smile DESABLE"));
			}
		}


		if (ps2x.ButtonPressed(PSB_R1)) {			// R1 Button Test
			playSoundBuzz(7);
			F_enableLightAuto=false;
			F_lightON=!F_lightON;

			if(F_lightON){
				FrontLightOnOff(true);
				Serial.println(F("==LIGHT ON"));
			}
			else if(!F_lightON){
				FrontLightOnOff(false);
				Serial.println(F("==LIGHT OFF"));
			}
		}


		if (ps2x.ButtonPressed(PSB_R2)) {			// Square Button Test 
			playSoundBuzz(7);
			F_enableLightAuto=!F_enableLightAuto;
			if(F_enableLightAuto){
				Serial.println(F("==Auto Light ENABLE"));
			}
			else if(!F_enableLightAuto){
				FrontLightOnOff(false);
				Serial.println(F("==Auto Light DESABLE"));
			}
		}


		if (ps2x.ButtonPressed(PSB_TRIANGLE)) {			// Triangle - Button Test 
			playSoundBuzz(7);
			if(!F_DanceBot && !F_ChoiceMusic){
				if(++nMusic > MAXNUM_MUSIC-1)			// set next track
					nMusic=0;
				getFaleNameFromScriptPGM(scripts_FaleNameMusic,nMusic,FaleExt_wav_scr);
				playFile(fileName,IGNORE_isPlAY);
				nMusicRedy=nMusic;						// set parm. redy track for save to EEPROM
			}
		}

		if (ps2x.ButtonPressed(PSB_L3)) {			// L3 Button Test 
			playSoundBuzz(7);
			if(!F_DanceBot && !F_ChoiceMusic){
				if(nMusicRedy != 255){
					SetChoiceMusic(nMusicRedy);		// save track to EEPROM 
					if(F_stop){
						pausePlayFile();
					}
					else if(!F_stop){
						stopPlayFile();
						playFileDisable();
					}
				}
			}
		}


		if (ps2x.ButtonPressed(PSB_R3)) {			// R3 Button Test 
			playSoundBuzz(7);
			if(!F_DanceBot && !F_ChoiceMusic){
				if(nMusicRedy != 255){
					nMusicRedy=255;					// reset parm. redy track for save to EEPROM
					Serial.println(F("res nMusicRedy"));
					if(F_stop){
						pausePlayFile();
					}
					else if(!F_stop){
						stopPlayFile();
						playFileDisable();
					}
				}
			}
		}


		if(!F_blockJoystick){ // block Joystick exstended ButtonPressed() functions

			if (ps2x.ButtonPressed(PSB_CIRCLE)){			// O - Circle Button Test
				playSoundBuzz(7);
				F_manual = !F_manual;
				if(F_manual){
					F_manual1=false;
					Serial.println(F("==MANUAL"));
				}
				else if(!F_manual){
					Serial.println(F("==AUTO"));
					speedML=180;
					speedMR=180;
				}
			}

			if (ps2x.ButtonPressed(PSB_SELECT)){
				playSoundBuzz(7);
				if(mode == ONE_JOYSTICK){
					mode=TWO_JOYSTICK;
					Serial.println(F("==TWO_JOYSTICK"));
				}
				else if(mode == TWO_JOYSTICK){
					mode=ONE_JOYSTICK;
					Serial.println(F("==ONE_JOYSTICK"));
				}
			}

			if(F_manual && (mode == ONE_JOYSTICK)){

				GetOneJoystick();

				if(Ly > 70){
					if(Lx < -70){
						resYX=Ly+Lx;
						if(resYX <= 0){
							speedML=30;
							speedMR=Ly;
						}
						else if(resYX > 0){
							speedML=resYX;
							speedMR=Ly;
						}
					}
					else if(Lx > 70){
						resYX=Ly-Lx;
						if(resYX <= 0){
							speedMR=30;
							speedML=Ly;
						}
						else if(resYX > 0){
							speedMR=resYX;
							speedML=Ly;
						}
					}
					else{
						speedMR=Ly;
						speedML=Ly;
					}
				}
				else if(Ly < -70){
					if(Lx < -70){
						resYX=Ly-Lx;
						if(resYX >= 0){
							speedML=30;
							speedMR=Ly;
						}
						else if(resYX < 0){
							speedML=resYX;
							speedMR=Ly;
						}
					}
					else if(Lx > 70){
						resYX=Ly+Lx;
						if(resYX >= 0){
							speedMR=30;
							speedML=Ly;
						}
						else if(resYX < 0){
							speedMR=resYX;
							speedML=Ly;
						}
					}
					else{
						speedMR=Ly;
						speedML=Ly;
					}
				}
				else if((Ly <= 70) && (Ly >= -70)){
					if(Lx < -70){
						speedMR= -Lx;
						speedML=  Lx;
					}
					else if(Lx > 70){
						speedMR= -Lx;
						speedML=  Lx;
					}
					else{
						speedMR=0;
						speedML=0;
					}
				}
				else{
					speedMR=0;
					speedML=0;
				}
			}
			else if(F_manual && (mode == TWO_JOYSTICK)){

				GetTwoJoystick();

				if(Ry > 70){
					if(Lx < -70){
						resYX=Ry+Lx;
						if(resYX <= 0){
							speedML=30;
							speedMR=Ry;
						}
						else if(resYX > 0){
							speedML=resYX;
							speedMR=Ry;
						}
					}
					else if(Lx > 70){
						resYX=Ry-Lx;
						if(resYX <= 0){
							speedMR=30;
							speedML=Ry;
						}
						else if(resYX > 0){
							speedMR=resYX;
							speedML=Ry;
						}
					}
					else{
						speedMR=Ry;
						speedML=Ry;
					}
				}
				else if(Ry < -70){
					if(Lx < -70){
						resYX=Ry-Lx;
						if(resYX >= 0){
							speedML=30;
							speedMR=Ry;
						}
						else if(resYX < 0){
							speedML=resYX;
							speedMR=Ry;
						}
					}
					else if(Lx > 70){
						resYX=Ry+Lx;
						if(resYX >= 0){
							speedMR=30;
							speedML=Ry;
						}
						else if(resYX < 0){
							speedMR=resYX;
							speedML=Ry;
						}
					}
					else{
						speedMR=Ry;
						speedML=Ry;
					}
				}
				else if((Ry <= 70) && (Ry >= -70)){
					if(Lx < -70){
						speedMR= -Lx;
						speedML=  Lx;
					}
					else if(Lx > 70){
						speedMR= -Lx;
						speedML=  Lx;
					}
					else{
						speedMR=0;
						speedML=0;
					}
				}
				else{
					speedMR=0;
					speedML=0;
				}
			}
			else if(!F_manual && (mode == ONE_JOYSTICK) && !F_TurnToOpening){

				GetOneJoystick();

				if((Ly > 70) || (Ly < -70)){
					motorsWrite(Ly, Ly);
					if(Ly > 70){
						F_forvard=true;
						F_reverce=false;
					}
					else if(Ly < -70){
						F_reverce=true;
						F_forvard=false;
					}
					F_manual1=true;
				}
				else if((Lx > 70) || (Lx < -70)){
					if(Lx > 70){
						motorsWrite(Lx, -Lx);
					}
					else if(Lx < -70){
						motorsWrite(Lx, -Lx);
					}
					F_manual1=true;
					F_forvard=false;
					F_reverce=false;
				}
				else if(F_manual1){
					motorsStop(); // stop the motors 
					//old_speedML=0; old_speedMR=0;
					F_forvard=false;
					F_reverce=false;
					F_manual1=false;
				}
			}
			else if(!F_manual && (mode == TWO_JOYSTICK) && !F_TurnToOpening){

				GetTwoJoystick();

				if((Ry > 70) || (Ry < -70)){
					motorsWrite(Ry, Ry);
					if(Ry > 70){
						F_forvard=true;
						F_reverce=false;
					}
					else if(Ry < -70){
						F_reverce=true;
						F_forvard=false;
					}
					F_manual1=true;
				}
				else if((Lx > 70) || (Lx < -70)){
					if(Lx > 70){
						motorsWrite(Lx, -Lx);
					}
					else if(Lx < -70){
						motorsWrite(Lx, -Lx);
					}
					F_manual1=true;
					F_forvard=false;
					F_reverce=false;
				} 
				else if(F_manual1){
					motorsStop(); // stop the motors 
					//old_speedML=0; old_speedMR=0;
					F_forvard=false;
					F_reverce=false;
					F_manual1=false;
				}
			}
			if(!F_manual1){
				ifForvard();
				ifReverce();
			}
		}// end, if (F_blockJoystick)
	}  // end, if((ps2x.Analog(1) & 0xf0) == 0x70), read PS2 controller 
	else {
		if (PS2ErrorCnt < MAX_PS2_ERROR_CNT)
			PS2ErrorCnt++;						// Increment the error count and if to many errors, turn off the robot.
		else if (F_powerOn){
			turnARobotOff();
			printARobotOff();
		}  
		ps2x.reconfig_gamepad();
	}
} // end, RobotControl::getPS2ControlInput


void RobotControl::GetOneJoystick(){

	Lx = (ps2x.Analog(PSS_LX)-128);
	Ly = -(ps2x.Analog(PSS_LY)-127);

	if(Lx > 5)
		Lx=map(Lx,6,127,70,255);
	else if(Lx < -5)
		Lx=map(Lx,-6,-128,-70,-255);
	if(Ly > 5)
		Ly=map(Ly,6,127,70,255);
	else if(Ly < -5)
		Ly=map(Ly,-6,-128,-70,-255);

	PrintLxLyRxRy(Lx,Ly,Rx,Ry);
}


void RobotControl::GetTwoJoystick(){

	Lx = (ps2x.Analog(PSS_LX)-128);
	Ry = -(ps2x.Analog(PSS_RY)-127);

	if(Lx > 5)
		Lx=map(Lx,6,127,70,255);
	else if(Lx < -5)
		Lx=map(Lx,-6,-128,-70,-255);

	if(Ry > 5)
		Ry=map(Ry,6,127,70,255);
	else if(Ry < -5)
		Ry=map(Ry,-6,-128,-70,-255);

	PrintLxLyRxRy(Lx,Ly,Rx,Ry);
}



void PrintLxLyRxRy(int Lx, int Ly, int Rx, int Ry){

#ifdef DEBUG // DEBUG JOYSTICK
	if (F_DebugOutput) {
		if (F_LxLyRxRy) {
			Serial.print(F("LxLyRxRy: "));
			Serial.print(Lx, DEC);
			Serial.print(F(" "));
			Serial.print(Ly, DEC);
			Serial.print(F(" "));
			Serial.print(Rx, DEC);
			Serial.print(F(" "));
			Serial.println(Ry, DEC);
		}
	}  
#endif
}

//-------------------------------------------------------------------------------
// Function to read inputs from the PS2 and then process any commands (FOR Play Sound ONLY)
//-------------------------------------------------------------------------------
//void RobotControl::getPS2ControlInput_s(void){
//
//	PS_PAD_key= -1;
//
//	// Then try to receive a packet of information from the PS2.
//	ps2x.read_gamepad();                            // read controller and set large motor to spin at 'vibrate' speed
//
//	if ((ps2x.Analog(1) & 0xf0) == 0x70) {
//
//	#ifdef DEBUG
//		if (F_DebugOutput) {
//			if (F_DebugPS2Input) {
//				// DEBUG PS2 INPUT
//				Serial.print(F("PS2 Input: "));
//				Serial.print(ps2x.ButtonDataByte(), HEX);
//				Serial.print(F(":"));
//				Serial.print(ps2x.Analog(PSS_LX), DEC);
//				Serial.print(F(" "));
//				Serial.print(ps2x.Analog(PSS_LY), DEC);
//				Serial.print(F(" "));
//				Serial.print(ps2x.Analog(PSS_RX), DEC);
//				Serial.print(F(" "));
//				Serial.println(ps2x.Analog(PSS_RY), DEC);
//			}
//		}  
//	#endif
//
//
//		if (ps2x.ButtonPressed(PSB_START)){			// Start Button Test 
//			playSound(7);
//			PS_PAD_key=BUTTON_MIDDLE;
//		}
//
//		if (ps2x.ButtonPressed(PSB_PAD_UP)){
//			playSound(7);
//			PS_PAD_key=BUTTON_UP;
//		}
//
//		if (ps2x.ButtonPressed(PSB_PAD_DOWN)){
//			playSound(7);
//			PS_PAD_key=BUTTON_DOWN;
//		}
//
//		if (ps2x.ButtonPressed(PSB_PAD_RIGHT)){
//			playSound(7);
//			F_powerOn=true;
//			PS_PAD_key=BUTTON_RIGHT;
//		}
//
//		if (ps2x.ButtonPressed(PSB_PAD_LEFT)){
//			playSound(7);
//			PS_PAD_key=BUTTON_LEFT;
//		}
//
//		if (ps2x.ButtonPressed(PSB_CROSS)) {			// X - Cross Button Test
//			playSound(7);								// [50\4000]
//			F_enableSaund=!F_enableSaund;
//			if(F_enableSaund){
//				Serial.println(F("==SAUND ENABLE"));
//			}
//			else if(!F_enableSaund){
//				Serial.println(F("==SAUND DESABLE"));
//			}
//		}
//
//		if (ps2x.ButtonPressed(PSB_SQUARE)) {			// Square Button Test 
//			playSound(7);
//			F_enableLightAuto=!F_enableLightAuto;
//			if(F_enableLightAuto){
//				Serial.println(F("==Auto Light ENABLE"));
//			}
//			else if(!F_enableLightAuto){
//				FrontLightOnOff(false);
//				Serial.println(F("==Auto Light DESABLE"));
//			}
//		}
//
//		if (ps2x.ButtonPressed(PSB_R1)) {			// R1 Button Test
//			playSound(7);
//			F_enableLightAuto=false;
//			F_lightON=!F_lightON;
//
//			if(!F_lightON){
//				FrontLightOnOff(true);
//				Serial.println(F("==LIGHT ON"));
//			}
//			else if(F_lightON){
//				FrontLightOnOff(false);
//				Serial.println(F("==LIGHT OFF"));
//			}
//		}
//
//		if (ps2x.ButtonPressed(PSB_CIRCLE)) {			// O - Circle Button Test
//		}
//
//		if (ps2x.ButtonPressed(PSB_TRIANGLE)) {			// Triangle - Button Test 
//			playSound(7);
//			if(!F_DanceBot && !F_ChoiceMusic){
//				getFaleNameFromScriptPGM(scripts_FaleNameMusic,nMusic,FaleExt_wav_scr);
//				playFile(fileName,IGNORE_isPlAY);
//				nMusicRedy=nMusic;						// set parm. redy track for save to EEPROM
//				if(++nMusic > MAXNUM_MUSIC)				// set next track
//					nMusic=0;
//			}
//		}
//
//		if (ps2x.ButtonPressed(PSB_L3)) {			// L3 Button Test 
//			playSound(7);
//			if(!F_DanceBot && !F_ChoiceMusic){
//				if(nMusicRedy != 255){
//					SetChoiceMusic(nMusicRedy);		// save track to EEPROM 
//					if(F_stop){
//						pausePlayFile();
//					}
//					else if(!F_stop){
//						stopPlayFile();
//						playFileDisable();
//					}
//				}
//			}
//		}
//
//
//		if (ps2x.ButtonPressed(PSB_R3)) {			// R3 Button Test 
//			playSound(7);
//			if(!F_DanceBot && !F_ChoiceMusic){
//				if(nMusicRedy != 255){
//					nMusicRedy=255;					// reset parm. redy track for save to EEPROM
//					Serial.println(F("res nMusicRedy"));
//					if(F_stop){
//						pausePlayFile();
//					}
//					else if(!F_stop){
//						stopPlayFile();
//						playFileDisable();
//					}
//				}
//			}
//		}
//		
//
//	}  // end, if((ps2x.Analog(1) & 0xf0) == 0x70), read PS2 controller 
//	else {
//		if (PS2ErrorCnt < MAX_PS2_ERROR_CNT)
//			PS2ErrorCnt++;                          // Increment the error count and if to many errors, turn off the robot.
//		else if (F_powerOn){
//			turnARobotOff();
//			printARobotOff();
//		}  
//		ps2x.reconfig_gamepad();
//	}
//} // end, RobotControl::getPS2ControlInput_s



bool ifForvard(){

	if((speedML > 0) && (speedMR > 0))
		F_forvard=true;
	else 
		F_forvard=false;
	return F_forvard;
}


bool ifReverce(){

	if((speedML < 0) && (speedMR < 0))
		F_reverce=true;
	else 
		F_reverce=false;
	return F_reverce;
}


void printARobotOff(){
#ifdef DEBUG
	DBGSerial.println(F("ARobot OFF!"));
#endif          
}


     


