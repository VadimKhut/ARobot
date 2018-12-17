#include "ArduinoRobot.h"
#include "RobotTextManager.h"
#include "scripts_Hello_User.h"

//************************************************************************************
extern uint8_t read8(File& f);
extern int16_t read16i(File& f);
extern uint16_t read16(File& f);
extern void keySelection(void);
extern void initFlagsForAnyState(void);
extern bool setFlagPrintState();
extern bool TerminalMonitor(void);
extern void playChoicedMusicFile();
extern bool ifForvard();
extern bool ifReverce();
extern void getFaleNameFromScriptPGM(const char* const* FaleName_script_P, uint8_t nStr, const char* FaleNameExt_scripts_P);
extern void Init_PS2();

extern int oldKey;
extern bool F_stop;
extern bool F_powerOn;
extern bool F_printState;
extern bool F_DebugOutput;
extern bool F_DebugDance;
extern byte stateMove;
extern byte old_stateMove;
extern bool F_contFromStop;
extern int16_t speedML;
extern int16_t speedMR;
extern long MwaitFrom;
extern long TimeRunDone;
extern bool F_ShowDebugPrompt;
extern bool F_blockJoystick;
extern bool F_blockContinue;
extern int8_t PS_PAD_key;
extern bool F_continueFOR;
extern bool F_forvard;
extern bool F_reverce;
extern char fileName[13];
extern char filenameWAW[13];
extern char filenameGPS[13];
extern byte cDisplayState_5s;
extern bool F_displSmilesEn;
extern bool F_displayState;
extern void timeSelector();

void PrintDebugDance();

uint16_t nSEQ;
uint16_t MwaitTime;
uint8_t Fr_Light;
uint8_t Led_Style;
uint8_t Led_StyleOld;
uint16_t Led_Wait;
uint16_t curSeq;
int calSpeedY;


//************************************************************************************




// Read and run file GP from SD card through the filename.
void RobotControl::PlayFileGP(uint8_t nStr){
	uint8_t Distance;
	stateMove=INIT_FOR_MOVE;

	getFaleNameFromScriptPGM(scripts_FaleNameMusic,nStr,FaleExt_wav_scr);
	strcpy(filenameWAW, fileName);						// fileName.wav --> filenameWAW
#ifdef DEBUG
	Serial.println(filenameWAW);
#endif
	getFaleNameFromScriptPGM(scripts_FaleNameMusic,nStr,FaleExt_gps_scr);
	strcpy(filenameGPS, fileName);						// fileName.gps --> filenameGPS
#ifdef DEBUG
	Serial.println(filenameGPS);
#endif
	while (true){
		switch (stateMove){
			case INIT_FOR_MOVE:
				if(F_displSmilesEn){
					textManager.HeadScreenMove();
					if(F_displSmilesEn){
						Robot.fill(WHITE);  
						Robot.stroke(WHITE);
						Robot.rect(0,68,128,80);
						Robot.stroke(BLACK);
						Robot._drawBMP_P(PSTR("Smile6m.bmp"),26,68);
					}
				}
				curSeq=0;
				nSEQ=1;
				F_powerOn=false;
				//F_stop=false;
				//F_contFromStop=false;
				//F_blockJoystick=true;
				//old_stateMove=INIT_FOR_MOVE;
				Led_StyleOld=252;
				// Open requested file on SD card
				if ((file=SD.open(filenameGPS)) == NULL) {   //Vad file.open -> file=SD.open
					stateMove=ERROR_STATE;
				}
				else{
					if(read16(file) == 0x5047) { // GP Seguence signature
						nSEQ=read16(file);// Max sequences
				      #ifdef DEBUG
						Serial.print(F("nSEQ="));
						Serial.println(nSEQ, DEC);
				      #endif
						stateMove=POWER_STATE_OFF;
					}
					else{
						stateMove=ERROR_STATE;
					}
				}
				Init_PS2();
				break;

			case ERROR_STATE:
				if(F_printState){
					Serial.println(F("==ERR"));
					Robot.leds_Stop();
					Robot.playFileDisable();
					if(F_displSmilesEn){
						Robot.fill(WHITE);  
						Robot.stroke(WHITE);
						Robot.rect(0,68,128,80);
						Robot.stroke(BLACK);
						Robot._drawBMP_P(PSTR("Smile2m.bmp"),26,68);
					}
					Robot.fill(WHITE);  
					Robot.stroke(WHITE);
					Robot.rect(0,18,128,24);
					Robot.stroke(BLACK);
					Robot.setTextSize(2);
					textManager.writeText_P(PSTR("ERROR"),1,1);
					Robot.setTextSize(1);
					cDisplayState_5s=0;
					F_displayState=true;
					if(F_displSmilesEn)
						Init_PS2();
				}
				break;

			case RETURN_STATE:
				if(F_printState){
					Serial.println(F("==RET"));
					Robot.leds_Stop();
					file.close();
				}
				return;

			case STOP_STATE:
				if(F_printState){
					Serial.println(F("==STOP"));
					Robot.fill(WHITE);  
					Robot.stroke(WHITE);
					Robot.rect(0,18,128,24);
					Robot.stroke(BLACK);
					Robot.setTextSize(2);
					textManager.writeText_P(PSTR("STOP"),1,1);
					Robot.setTextSize(1);
					cDisplayState_5s=0;
					F_displayState=true;
				}
				break;

			case POWER_STATE_OFF:
				if(F_printState){
					Serial.println(F("==PWOF"));
					Robot.fill(WHITE);  
					Robot.stroke(WHITE);
					Robot.rect(0,18,128,24);
					Robot.stroke(BLACK);
					Robot.setTextSize(2);
					textManager.writeText_P(PSTR("STAND BY"),1,1);
					Robot.setTextSize(1);
					cDisplayState_5s=0;
					F_displayState=true;
				}
			  #ifdef OPT_TERMINAL_MONITOR  
				TerminalMonitor();           
			  #endif
				break;
				
			case RUN_DANCE:
				if(F_printState){
					Serial.println(F("==RUN_DANCE"));
					Robot.fill(WHITE);  
					Robot.stroke(WHITE);
					Robot.rect(0,18,128,24);
					Robot.stroke(BLACK);
					Robot.setTextSize(2);
					textManager.writeText_P(PSTR("RUN!"),1,1);
					Robot.setTextSize(1);
					cDisplayState_5s=0;
					F_displayState=true;
					if(!F_contFromStop)
						playFile(filenameWAW,IGNORE_isPlAY);
				}
				for(; curSeq < nSEQ; curSeq++){
					if(!F_contFromStop){
						speedML=read16i(file);
						speedMR=read16i(file);
						MwaitTime=read16(file);
						Fr_Light=read8(file);
						Led_Style=read8(file);
						Led_Wait=read16(file);
						if(Led_StyleOld != Led_Style){
							leds_Show(Led_Style,255,1,Led_Wait);
						}
					}
					else if(F_contFromStop){
						MwaitTime=MwaitTime-TimeRunDone; //calculate the remaining time 
						leds_Pause_Show();
						F_contFromStop=false;
					}
					motorsWrite(speedML, speedMR);
					FrontLightOnOff(Fr_Light);
					Led_StyleOld=Led_Style;

				#ifdef DEBUG
					if (F_DebugOutput) {
						if (F_DebugDance){
							PrintDebugDance();
						}
					}
				#endif
					MwaitFrom=millis(); 
					while(TimeRunDone=(millis()-MwaitFrom) < MwaitTime){
						keySelection();
						getPS2ControlInput();
						if(F_stop)
							break;

						ifForvard();
						ifReverce();

						if(F_forvard){
							Distance=Robot.UScheckDistanceFr();
							if(Distance == 1){								// (< 30cm)
								Robot.motorsStop();
								F_continueFOR=true;
								break;
							}
							if(Robot.getWiskerLF() == 2){
								Robot.motorsStop();
								F_continueFOR=true;
								break;
							}
							if(Robot.getWiskerRF()== 2){
								Robot.motorsStop();
								F_continueFOR=true;
								break;
							}
						}
						else if(F_reverce){
							Distance=Robot.UScheckDistanceRv();
							if(Distance == 1){								// (< 30cm)
								Robot.motorsStop();
								F_continueFOR=true;
								break;
							}
							if(Robot.getWiskerLR() == 2){
								Robot.motorsStop();
								F_continueFOR=true;
								break;
							}
							if(Robot.getWiskerRR() == 2){
								Robot.motorsStop();
								F_continueFOR=true;
								break;
							}
						}
						textManager.printTimeTempC(15,12,false);
						if(Robot.F_enableCheckLight_1s)
							Robot.checkLight();
						if(Robot.F_enableCheckVBat_1s)
							Robot.checkVoltage();
						delay(10);
					}//while delay MwaitTime...
					if(F_stop)
						break;
					if(F_continueFOR)		// curSeq++
						continue;
				} //for nSEQ
		}//switch (stateMove)
		if(curSeq >= nSEQ){		// sequence ended
			F_powerOn=false;
			F_stop = false;
			stopPlayFile();
			playFileDisable();
			motorsStop();
			leds_Stop();
			F_ShowDebugPrompt=true; // for debug
			stateMove=RETURN_STATE;
		}
		keySelection();
		getPS2ControlInput();
		F_printState=setFlagPrintState();
		old_stateMove=stateMove;
		textManager.printTimeTempC(15,12,false);
		if(Robot.F_enableCheckLight_1s)
			Robot.checkLight();
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		timeSelector();
		delay(10);
	}// while(1)
}






void wheelCalibration(){
	int val;
	uint8_t key;
	int speedM=0;

	initFlagsForAnyState();

	calSpeedY=0; calSpeedY=0;
	textManager.oldValue=40;

	Robot._drawBMP_P(PSTR("lf.bmp"),0,0); //display background image

	textManager.HeadScreenDance();

	textManager.writeText_P(PSTR("1. Put Robot on a\n flat surface"),2,1);
	textManager.writeText_P(PSTR("2. Adjust speed\n with the joystick"),4,1);
	textManager.writeText_P(PSTR("3. Use screwdriver\n on the bottom trim"),6,1);
	key=Robot.waitContinueAnyKey_T();
	if(key == BUTTON_LEFT ||PS_PAD_key == BUTTON_LEFT){
		return;
	}
	Robot.fill(255,255,255);
	Robot.stroke(255,255,255);
	Robot.rect(0, 20, 128, 60); // erase the previous text
	Robot.stroke(0, 0, 0);
	textManager.writeText_P(PSTR("- Robot turns left,\n screw it clockwise;"),2,1);
	textManager.writeText_P(PSTR("- Turns right, screw it ct-colockwise;"),4,1);
	textManager.writeText_P(PSTR("4. Repeat 4 until\n going straight"),6,1);
	key=Robot.waitContinueAnyKey_T();
	if(key == BUTTON_LEFT ||PS_PAD_key == BUTTON_LEFT){
		return;
	}

	Robot.clearScreen();
	textManager.HeadScreenDance();
	playChoicedMusicFile();

	textManager.writeText_P(PSTR("Wheel Calibration"),2,1);

	while(true){

		if(calSpeedY > 70)
			speedM=calSpeedY;
		else if(calSpeedY < -70)
			speedM=calSpeedY;
		else
			speedM=0;

		Robot.motorsWrite(speedM, speedM);

		//read value of the pot on motor baord,to clibrate the wheels
		val=Robot.trimRead();
		if(val != -1)
			val=map(val,0,1023,-30,30);

		// print the values to the screen
		textManager.debugPrint(val,6,10);

		keySelection();
		Robot.getPS2ControlInput();
		if(!F_powerOn){
			return;
		}
		if(F_stop && F_contFromStop){ //pause
			F_contFromStop=false;
		}
		else if(!F_stop && F_contFromStop){ //continue
			F_contFromStop=false;
		}
		textManager.printTimeTempC(15,12,false);
		if(Robot.F_enableCheckLight_1s)
			Robot.checkLight();
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		delay(10);
	}
}




#ifdef DEBUG
void PrintDebugDance(){

	Serial.print(F("CurSeq="));
	Serial.println(curSeq, DEC);
	Serial.print(F(" "));
	Serial.print(speedML, DEC);
	Serial.print(F(" "));
	Serial.print(speedMR, DEC);
	Serial.print(F(" "));
	Serial.print(MwaitTime, DEC);
	Serial.print(F(" "));
	Serial.print(Fr_Light, DEC);
	Serial.print(F(" "));
	Serial.print(Led_Style, DEC);
	Serial.print(F(" "));
	Serial.println(Led_Wait, DEC);
}

#endif
