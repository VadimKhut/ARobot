//--------------------------------------------
// R_Mega_B_SD_SD_Tpcm_6_1 - Midle bord
// ARobot_Top_Core_TPcm_4_1 - Tob bord
// ARobot_Motor_Core_NeoP_6_3 - Botton bord
//--------------------------------------------


#include <Ultrasonic.h>
#include <PS2X_lib.h>
#include "DS1307.h"
#include "ArduinoRobot.h" 
#include "RobotTextManager.h"  
#include "scripts_Hello_User.h"

//************************************************************************************
#define TO_USfr				1
#define TO_USrv				1
#define PrCheck_US_fr		2
#define PrCheck_US_rv		2

extern uint16_t curSeq;
extern void wheelCalibration();
void printReadFromEE_track(uint8_t track);

int8_t oldKey; 
int8_t PS_PAD_key;
bool F_DebugPS2Input=false;
bool F_DebugTemperatC=false;
bool F_DebugVlight=false; bool F_LxLyRxRy=false;
bool F_DebugWiskers=false; bool F_DebugWiskersTest=false;
bool F_ShowDebugPrompt=true; bool F_DebugDance=false;
bool F_DebugOutput; bool F_UDistance=false; bool F_DebugRun=false;
bool F_contFromStop; bool F_DebugVoltage;
char buffer[21];	//for storing user name
char buffer1[5];	//for storing 
uint8_t PosMainMenu=0; uint8_t PosServiceMenu=0; uint8_t PosDanceMenu=0;
uint8_t PosChoisMusicMenu=0;
byte stateMove; byte old_stateMove;
int speedML; int speedMR; int old_speedML=0; int old_speedMR=0; int prTurn; int PS2_Error;
bool F_Skip_DoOpening;  bool F_afterStopDelay;
bool F_powerOn; bool F_stop; bool F_USstop; bool F_forvard;
bool F_slowFr; bool F_TurnToOpening; bool F_ChoiceMusic; bool F_DanceBot;
bool F_wiskerFr;  bool F_reverce; bool F_wiskerRv; bool F_enableSaund;
bool F_slowRv; bool F_wiskersAny;  bool F_manual; bool F_manual1;
bool F_wiskerLF;bool F_wiskerRF;bool F_wiskerLR;bool F_wiskerRR;
bool F_printState; bool F_blockJoystick; bool F_blockContinue;
bool F_continueFOR; bool F_skeepWaitAnyTime=false; bool F_enableLightAuto;
bool F_displayState; bool F_displ_ErrorVolt; bool F_displSmilesEn;
uint8_t US_BusyStatusFr=0; uint8_t US_BusyStatusRv=0;
uint32_t old_stopDelay; uint32_t old_slowDelay;
byte old_second; byte old_minTurn; byte c_minuteTurn=0; byte cDisplayState_5s; byte cDisplayErrorVolt_15s;
long MwaitFrom; long TimeRunDone;
int commands[20];					// array for storing commands
int nF=1;							// Current gallery sequence counter
int modeBR=1;						// Current mode "tilt"

// name the modes
#define CONTROL_MODE_KEY		0
#define CONTROL_MODE_COMPASS	1
#define NUM_PICS				4	//Total number of pictures in Gallery

char fileName[13];
char filenameWAW[13];
char filenameGPS[13];

uint8_t nMusic=0;
uint8_t nMusicRedy;
//************************************************************************************


void setup(){
	//necessary initialization sequence
	Serial.begin(DEBUG_BAUD);
	Robot.begin();
	Robot.beginTFT();
	Robot.beginSD();
	Robot.digitalWrite(ORS_ON,LOW); // ORS OFF
	pinMode(LIGHT_ON, OUTPUT);
	digitalWrite(LIGHT_ON, LOW);	// front light off
	pinMode(TMP_REDY, INPUT);		// TMPPCM_REDY pin to input
	Robot.F_lightON=false;
	textManager.readScratchPadDS18B20();
	textManager.setResolution_9bit();
	textManager.readScratchPadDS18B20();
	initFlagsForAnyState();
	nMusicRedy=255; // for start up music
	F_displSmilesEn=true;

#ifdef DEBUG
	DBGSerial.println(F("Welcome"));
#endif          

	Init_PS2();

	nMusic=Robot.readByte(ADDR_CHOICE_MUSIC); // get namber track from EEPROM for play music 

#ifdef DEBUG
	printReadFromEE_track(nMusic);
#endif

	// show the logos from the SD card
	// draw "lg0.bmp" and "lg1.bmp" on the screen   
	Robot.displayLogos();
	if(!F_powerOn){
		return;
	}

	Init_PS2();

}









void loop(){

	initFlagsForAnyState();
	PosMainMenu=textManager.inputMainMenu(3,2,PosMainMenu);

	switch(PosMainMenu){
		case RUN:
			Move();
			break;
		case DISCO_BOT:
			DanceMenu();
			break;
		case PICTURE_BROWSER:
			PictureBrowser();
			break;
		case LINE_FOLLOW:
			Line_Follow();
			break;
		case RESCUE_BOT:
			Rescue_Bot();
			break;
		case RUN_PATTERN:
			Run_pattern();
			break;
		case SERVICE:
			serviceMenu();
			break;

	}
}


//--------------------------------------------------------------------------------------
void DanceMenu(){

	//initFlagsForAnyState();
	F_ChoiceMusic=true;
	Robot.FrontLightOnOff(false);
	F_enableLightAuto=false;

	PosDanceMenu=textManager.inputDanceMenu(3,2,PosDanceMenu);
	if(PosDanceMenu < DANCE_MENU_LINE-1){
		DanceBot(PosDanceMenu);
	}
}


void ChoiceMusicMenu(){

	//initFlagsForAnyState();

	F_ChoiceMusic=true;
	PosChoisMusicMenu=textManager.inputChoiceMusicMenu(3,2,PosChoisMusicMenu);
	F_ChoiceMusic=false;

	if(PosChoisMusicMenu < DANCE_MENU_LINE-1)
		SetChoiceMusic(PosChoisMusicMenu);
}


//Save track to EEPROM 
void SetChoiceMusic(uint8_t track){

	nMusicRedy=255;					// reset parm. redy track for save to EEPROM
	Robot.writeByte(ADDR_CHOICE_MUSIC,track);
#ifdef DEBUG
	Serial.print(F("SaveToEE_track: "));
	Serial.print(track, DEC);
	track=Robot.readByte(ADDR_CHOICE_MUSIC);
	printReadFromEE_track(track);
#endif
	//Serial.print(F("   "));
	//Serial.print(F("ReadFromEE_track: "));
	//Serial.println(track, DEC);
}







void DanceBot(uint8_t nStr){

	initFlagsForAnyState();
	F_DanceBot=true;

	textManager.HeadScreenDance();

	Robot.PlayFileGP(nStr);
	F_DanceBot=false;
}





//--------------------------------------------------------------------------------------
void Run_pattern(){

	//initFlagsForAnyState();

	Robot._drawBMP_P(PSTR("intro.bmp"),0,0); //display background image

	textManager.HeadScreenDance();

	initCommands(); // remove commands from the array
	addCommands();  // add commands to the array

	WaitAnyTime(1000,0); // delay(1000);
	if(!F_powerOn)
		return;

	executeCommands(); // follow orders
	if(!F_powerOn)
		return;

	Robot.motorsStop();

#ifdef DEBUG
	Serial.println(F("\nEnd execute"));
#endif

	textManager.writeText_P(PSTR("Done!"),12, 3);
	WaitAnyTime(1000,0); // delay(1000);
	Robot.stopPlayFile();
	Robot.playFileDisable();
	Robot.leds_Stop();
}


void initCommands(){
	for(int i=0; i < 20; i++)
		commands[i]= -1;
}


// add commands to the array
void addCommands(){
	int key;

	textManager.writeText_P(PSTR("1. Press buttons to\n add commands.\n\n 2. Middle to finish."),2,1);

	// read the buttons' state
	for(int i=0; i < 20;){  //max 20 commands

		switch(key=Robot.keyboardRead()){

			case BUTTON_MIDDLE:
				Robot.playSoundBuzz(7);
				break;
			case BUTTON_NONE:
				textManager.printTimeTempC(15,12,false);
				if(Robot.F_enableCheckVBat_1s)
					Robot.checkVoltage();
				delay(10);
				oldKey=key;
				continue;
			default:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					commands[i]=key;		// save the button to the array
					PrintCommandI(i, 56);	// print the command on the screen
					i++;
				}
		} // switch()
		oldKey=key;
		if(key == BUTTON_MIDDLE){  //finish input
			break;
		}

	} // for()
}


// run through the array and move the robot
void executeCommands() {
	uint8_t Distance;
	F_continueFOR=false;

	playChoicedMusicFile();
	textManager.writeText_P(PSTR("Excuting..."),7,1);

	// read through the array and move accordingly
	for(int i=0; i < 20; i++){


	#ifdef DEBUG
		Serial.print(commands[i], DEC);
		Serial.print(F(" "));
	#endif


		switch(commands[i]){
			case BUTTON_LEFT:
				Robot.leds_Show(CASCADE_UP,SHOW_CONTINUE,1,100);
				Robot.turn(-90);
				break;
			case BUTTON_RIGHT:
				Robot.leds_Show(CASCADE_DOUN,SHOW_CONTINUE,1,100);
				Robot.turn(90);
				break;
			case BUTTON_UP:
				Robot.leds_Show(COLOR_WIPE_G_UP,SHOW_CONTINUE,1,100);
				Robot.motorsWrite(180, 180);
				break;
			case BUTTON_DOWN:
				Robot.leds_Show(COLOR_WIPE_G_DOUN,SHOW_CONTINUE,1,100);
				Robot.motorsWrite(-180, -180);
				break;
			case BUTTON_NONE: //(-1)
				return;
		} // end switch()

		Robot.stroke(255,0,0);
		PrintCommandI(i, 86);

		MwaitFrom=millis();
		while(millis()-MwaitFrom < 1000 || F_stop){ // delay(1000);
			keySelection();
			Robot.getPS2ControlInput();
			if(!F_powerOn)
				return;
			if(F_stop && F_contFromStop){ //pause
				//F_blockContinue=true;
				F_contFromStop=false;
			}
			else if(!F_stop && F_contFromStop){ //continue
				F_contFromStop=false;
				switch(commands[i]) { //restore command
					case BUTTON_LEFT:
						Robot.leds_Show(CASCADE_UP,SHOW_CONTINUE,1,100);
						Robot.turn(-90);
						break;
					case BUTTON_RIGHT:
						Robot.leds_Show(CASCADE_DOUN,SHOW_CONTINUE,1,100);
						Robot.turn(90);
						break;
					case BUTTON_UP:
						Robot.leds_Show(COLOR_WIPE_G_UP,SHOW_CONTINUE,1,100);
						Robot.motorsWrite(255, 255);
						break;
					case BUTTON_DOWN:
						Robot.leds_Show(COLOR_WIPE_G_DOUN,SHOW_CONTINUE,1,100);
						Robot.motorsWrite(-255, -255);
						break;
					case BUTTON_NONE: //(-1)
						Robot.motorsStop();
						return;
				}
				MwaitFrom=millis(); //reset time
			}
			if(commands[i] == BUTTON_UP){
				Distance=Robot.UScheckDistanceFr();
				if(Distance == 1){								// (< 30cm)
					Robot.motorsStop();
					F_continueFOR=true;
					break;
				}
				if(Robot.getWiskerLF()){
					Robot.motorsStop();
					F_continueFOR=true;
					break;
				}
				if(Robot.getWiskerRF()){
					Robot.motorsStop();
					F_continueFOR=true;
					break;
				}
			}

			else if(commands[i] == BUTTON_DOWN){
				Distance=Robot.UScheckDistanceRv();
				if(Distance == 1){								// (< 30cm)
					Robot.motorsStop();
					F_continueFOR=true;
					break;
				}
				if(Robot.getWiskerLR()){
					Robot.motorsStop();
					F_continueFOR=true;
					break;
				}
				if(Robot.getWiskerRR()){
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
		} // end while()
		if(F_continueFOR)		// i++
			continue;
		// stop moving for a second
		Robot.motorsStop();


		WaitAnyTime(1000,0); // delay(1000);
		if(!F_powerOn)
			return;
	} //end for()
}


// convert the button press to a single character
char keyToChar(int key){
	
	switch(key){
		case BUTTON_LEFT:
			return '<';
		case BUTTON_RIGHT:
			return '>';
		case BUTTON_UP:
			return '^';
		case BUTTON_DOWN:
			return 'v';
	}
}


// display a command
void PrintCommandI(int i, int originY){
	
	Robot.text(keyToChar(commands[i]),i%14*8+5,i/14*10+originY);
}





//--------------------------------------------------------------------------------------
void Line_Follow(){
	long res;
	uint8_t key;

	//initFlagsForAnyState();

	Robot._drawBMP_P(PSTR("lf.bmp"),0,0); //display background image

	textManager.HeadScreenDance();

	textManager.writeText_P(PSTR("Line Following\n\n Place the robot on\n the track and \n see it run"),2,1);
	textManager.writeText_P(PSTR("Press the middle\n button to start..."),7,1);
	key=Robot.waitContinueAnyKey_T();
	if(key == BUTTON_LEFT || PS_PAD_key == BUTTON_LEFT){
		return;
	}

	playChoicedMusicFile();
	Robot.leds_Show();

	// Robot.lineFollowConfig(11,5,50,10);
	Robot.lineFollowConfig(14,9,70,10);

	Robot.digitalWrite(ORS_ON,HIGH); // ORS ON

	//set the motor board into line-follow mode
	Robot.setMode(MODE_LINE_FOLLOW);  

	 (255, 255, 255);
	Robot.stroke(255, 255, 255);
	Robot.rect(0, 0, 128, 90); // erase the previous text
	Robot.stroke(0, 0, 0);
	textManager.writeText_P(PSTR("Start"),2,1);

	textManager.writeText_P(PSTR("Time passed:"),4,1);

	MwaitFrom=millis(); // keep track of the elapsed time
	while(!Robot.isActionDone()) {  //wait for the finish signal

		if(!F_stop){
			res=millis()-MwaitFrom+TimeRunDone;;
			textManager.debugPrint(res,4,14);  // show how much time has passed
		}
		keySelection();
		Robot.getPS2ControlInput();
		if(!F_powerOn){
			Robot.setMode(MODE_SIMPLE);
			Robot.digitalWrite(ORS_ON,LOW); // ORS OFF
			return;
		}
		if(F_stop && F_contFromStop){ //pause
			Robot.pauseMode(1);
			TimeRunDone=res;
			F_contFromStop=false;
		}
		else if(!F_stop && F_contFromStop){ //continue
			Robot.pauseMode(0);
			MwaitFrom=millis();
			F_contFromStop=false;
		}
		textManager.printTimeTempC(15,12,false);
		if(Robot.F_enableCheckLight_1s)
			Robot.checkLight();
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		delay(10);
	} //end while

	//found a black line perpendicular to way
	Robot.digitalWrite(ORS_ON,LOW); // ORS OFF
	textManager.writeText_P(PSTR("Done!"),6,1);
	Robot.waitContinueKey_T();
	Robot.stopPlayFile();
	Robot.playFileDisable();
}





//--------------------------------------------------------------------------------------
void Rescue_Bot(){
	uint8_t key;

	//initFlagsForAnyState();

	//display background image
	Robot._drawBMP_P(PSTR("lf.bmp"),0,0); 
	
	textManager.HeadScreenDance();

	textManager.writeText_P(PSTR("Rescue\n\n Place the robot on\n the rescue track\n pushing the\n obstacles away"),2,1);
	textManager.writeText_P(PSTR("Press the middle\n button to start..."),7,1);
	key=Robot.waitContinueAnyKey_T();
	if(key == BUTTON_LEFT ||PS_PAD_key == BUTTON_LEFT){
		return;
	}

	// Robot.lineFollowConfig(11,5,50,10);
	Robot.lineFollowConfig(14,9,50,10);

	playChoicedMusicFile();
	Robot.leds_Show();

	Init_PS2();

	do{
		Robot.fill(255,255,255);
		Robot.stroke(255,255,255);
		Robot.rect(0, 0, 128, 90); // erase the previous text

		textManager.writeText_P(PSTR("Start"),2,1);
		TimeRunDone=0;
		// run the rescue sequence  
		rescueSequence();
		if(!F_powerOn){
			return;
		}
		// find the track again
		goToNext();
		if(!F_powerOn){
			return;
		}
	}while(1);

	// write status on the screen
	//textManager.writeText_P(PSTR("Done!"),6,1);
}


void rescueSequence(){
	long res;

	Robot.digitalWrite(ORS_ON,HIGH); // ORS ON
	//set the motor board into line-follow mode
	Robot.setMode(MODE_LINE_FOLLOW);  
	
	textManager.writeText_P(PSTR("Time passed:"),3,1);

	MwaitFrom=millis(); // keep track of the elapsed time
	while(!Robot.isActionDone()){  // wait until it is no longer following the line
	
		if(!F_stop){
			res=millis()-MwaitFrom+TimeRunDone;
			textManager.debugPrint(res,3,14);  // show how much time has passed
		}
		keySelection();
		Robot.getPS2ControlInput();
		if(!F_powerOn){
			Robot.setMode(MODE_SIMPLE);
			Robot.digitalWrite(ORS_ON,LOW); // ORS OFF
			return;
		}
		if(F_stop && F_contFromStop){ //pause
			Robot.pauseMode(1);
			TimeRunDone=res;
			F_contFromStop=false;
		}
		else if(!F_stop && F_contFromStop){ //continue
			Robot.pauseMode(0);
			MwaitFrom=millis();
			F_contFromStop=false;
		}
		textManager.printTimeTempC(15,12,false);
		if(Robot.F_enableCheckLight_1s)
			Robot.checkLight();
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		delay(10);
	} // end while()

	Robot.digitalWrite(ORS_ON,LOW); // ORS OFF
	textManager.writeText_P(PSTR("Found obstacle"),4,1);

	WaitAnyTime(1000,0); // delay(1000);
	if(!F_powerOn)
		return;

	// do the rescue operation
	doRescue();
	if(!F_powerOn)
		return;

	WaitAnyTime(1000,0); // delay(1000);
}


void doRescue(){

	// Reached the endline, engage the target
	Robot.motorsWrite(200,200);

	WaitAnyTime(250,1); // delay(250);
	if(!F_powerOn)
		return;

	Robot.motorsStop();


	WaitAnyTime(1000,0); // delay(1000);
	if(!F_powerOn)
		return;

	// Turn the robot 
	Robot.turn(90);
	if(F_stop)
		F_blockContinue=true;

	Robot.motorsStop();

	WaitAnyTime(1000,0); // delay(1000);
	if(!F_powerOn)
		return;

	// Move forward
	Robot.motorsWrite(200,200);

	WaitAnyTime(1000,1); // delay(1000);
	if(!F_powerOn)
		return;

	Robot.motorsStop();

	WaitAnyTime(1000,0); // delay(1000);
	if(!F_powerOn)
		return;

	// move backwards, leave the target
	Robot.motorsWrite(-200,-200);

	WaitAnyTime(500,1); // delay(500);
	if(!F_powerOn)
		return;

	Robot.motorsStop();
}


void goToNext(){

	// Turn the robot 
	Robot.turn(-90);
	if(F_stop)
		F_blockContinue=true;
	Robot.motorsStop();
	if(!F_stop)
		textManager.writeText_P(PSTR("Found track"),5,1);

	WaitAnyTime(1000,1); // delay(1000);
}


void WaitAnyTime(uint16_t wait_time, bool block){

	Init_PS2();

#ifdef DEBUG
	Serial.print(F("Wait "));
	Serial.println(wait_time, DEC);
#endif

	MwaitFrom=millis();
	while(millis()-MwaitFrom < wait_time || F_stop){ // delay(wait_time);
		keySelection();
		Robot.getPS2ControlInput();
		if(!F_powerOn)
			return;
		if(F_skeepWaitAnyTime){		// BUTTON_RIGHT or PAD_RIGHT
			F_skeepWaitAnyTime=false;
			F_stop=false;
			break;
		}
		if(F_stop && F_contFromStop){ //pause
			if(block)
				F_blockContinue=true;
			F_contFromStop=false;
		}
		else if(!F_stop && F_contFromStop){ //continue
			MwaitFrom=millis();
			F_contFromStop=false;
		}
		textManager.printTimeTempC(15,12,false);
		if(Robot.F_enableCheckLight_1s)
			Robot.checkLight();
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		delay(20);
	}
}





//--------------------------------------------------------------------------------------
void PictureBrowser(){
	uint8_t key;
	int8_t err;

	//initFlagsForAnyState();
	Robot.FrontLightOnOff(false);
	F_enableLightAuto=false;

	Robot._drawBMP_P(PSTR("init3.bmp"),0,0);

	textManager.writeText_P(PSTR("The gallery"),2,2);
	textManager.writeText_P(PSTR("has 2 modes. In"),3,1);
	textManager.writeText_P(PSTR("keyboard mode, L/R"),4,1);
	textManager.writeText_P(PSTR("key for switching"),5,1);
	textManager.writeText_P(PSTR("pictures, U/D key"),6,1);
	textManager.writeText_P(PSTR("for changing modes"),7,1);

	textManager.HeadScreenDance();

	key=Robot.waitContinueAnyKey_T();
	if(key == BUTTON_LEFT ||PS_PAD_key == BUTTON_LEFT){
		return;
	}

	Robot.clearScreen();
	Robot._drawBMP_P(PSTR("pb.bmp"),0,0);


	textManager.writeText_P(PSTR("In tilt mode,"),2,2);
	textManager.writeText_P(PSTR("quickly tilt the"),3,1);
	textManager.writeText_P(PSTR("robot to switch"),4,1);
	textManager.writeText_P(PSTR("pictures"),5,1);

	textManager.HeadScreenDance();

	key=Robot.waitContinueAnyKey_T();
	if(key == BUTTON_LEFT ||PS_PAD_key == BUTTON_LEFT){
		return;
	}

	playChoicedMusicFile();
	Init_PS2();

	while(true){

		strcpy_P(buffer, PSTR("pic"));		// "pic" --> buffer
		itoa(nF,buffer1,10);				// int nF --> (str*)buffer1
		strcat(buffer,buffer1);				// buffer+buffer1 = pic+XX= picXX
		strcpy_P(buffer1, PSTR(".bmp"));	// ".bmp" --> buffer1
		strcat(buffer,buffer1);				// buffer+buffer1 = picXX+.bmp = picXX.bmp
	#ifdef DEBUG
		Serial.println(buffer);
	#endif
		//img to be displayed
		err=Robot._drawBMP(buffer,0,0);
		if(err != 1){
			nF=1;
	#ifdef DEBUG
		Serial.println(F("nF=1"));
	#endif
		} 

		textManager.HeadScreenDance();

		Init_PS2();

		// change control modes
		switch(modeBR) { 
			case CONTROL_MODE_COMPASS:
				PicBrCompassControl(3);
				break;
			case CONTROL_MODE_KEY:
				PicBrKeybControl();
				break;
		}
		if(F_stop){
			while(true){
				keySelection();
				Robot.getPS2ControlInput();
				if(!F_powerOn)
					return;
				else if(!F_stop && F_contFromStop){ //continue
					F_contFromStop=false;
					break;
				}
				textManager.printTimeTempC(15,12,false);
				if(Robot.F_enableCheckLight_1s)
					Robot.checkLight();
				if(Robot.F_enableCheckVBat_1s)
					Robot.checkVoltage();
				delay(10);
			}
		}


	}// end while(true)
}


void PicBrKeybControl(){
	int key;

	#ifdef DEBUG
		Serial.println(F("keyboard"));
	#endif

	//Use buttons to control the gallery
	while(true) {
		//Use buttons to control the gallery
		switch(key=Robot.keyboardRead()){
			case BUTTON_LEFT:  // display previous picture
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					if(--nF < 1)
						nF = NUM_PICS;
					oldKey=key;
					return;
				}
				break;
			case BUTTON_MIDDLE:  // do stop/pause
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					Serial.println(F("stop/pause"));
					F_stop=true;
					if(nMusicRedy == 255)
						Robot.pausePlayFile();
					//Robot.pausePlayFile();
					//Robot.leds_Stop();
					oldKey=key;
					return;
				}
				break;
			case BUTTON_RIGHT:  // display next picture
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					if(++nF > NUM_PICS)
						nF=1;
					oldKey=key;
					return;
				}
				break;
			case BUTTON_UP:  // change mode
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					changeMode(-1);
					oldKey=key;
					return;
				}
				break;
			case BUTTON_DOWN:  // change mode
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					changeMode(1);
					oldKey=key;
					return;
				}
				break;
		} // end switch()
		oldKey=key;
		//Use PS2 PAD to control the gallery
		Robot.getPS2ControlInput();
		switch(PS_PAD_key){
			case BUTTON_LEFT:  // display previous picture
				PS_PAD_key= -1;
				if(--nF < 1)
					nF = NUM_PICS;
				return;
			case BUTTON_MIDDLE:  // do stop/pause
				PS_PAD_key= -1;
				F_stop=true;
				//Robot.pausePlayFile();
				if(nMusicRedy == 255)
					Robot.pausePlayFile();
				//Robot.leds_Stop();
				return;
			case BUTTON_RIGHT:  // display next picture
				PS_PAD_key= -1;
				if(++nF > NUM_PICS)
					nF = 1;
				return;
			case BUTTON_UP:  // change mode
				PS_PAD_key= -1;
				changeMode(-1);
				return;
			case BUTTON_DOWN:  // change mode
				PS_PAD_key= -1;
				changeMode(1);
				return;
		} // end switch()
		textManager.printTimeTempC(15,12,0);
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		delay(10);
	} // end while(1)
}


// if controlling by the compass (Tilt)
void PicBrCompassControl(int change){
	int key; int diff;
	int prevRead;

	#ifdef DEBUG
		Serial.println(F("Tilt"));
	#endif

	// Rotate the robot to change the pictures
	while(true) {
		// read the value of the compass
		prevRead=Robot.compassRead();

//#ifdef DEBUG
//		if(Robot.F_enableCheckVBat_1s){
//			Serial.print(F("Comp= "));
//			Serial.print(prevRead, DEC);
//		}
//#endif

		//get the change of angle
		diff=Robot.compassRead()-prevRead;
		delay(10);

//#ifdef DEBUG
//		if(Robot.F_enableCheckVBat_1s){
//			Serial.print(F(", dif= "));
//			Serial.println(diff, DEC);
//			//Robot.F_enableCheckVBat_1s=false;
//		}
//#endif

		if(diff > 180)
			diff -= 360;
		else if(diff < -180)
			diff += 360;

		if(abs(diff) > change){
			if(++nF > NUM_PICS)
				nF = 1;
			return;
		}
		// chage modes, if buttons are pressed
		switch(key=Robot.keyboardRead()){
			case BUTTON_UP:  // change mode
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					changeMode(-1);
					oldKey=key;
					return;
				}
				break;
			case BUTTON_DOWN:  // change mode
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					changeMode(1);
					oldKey=key;
					return;
				}
				break;
			case BUTTON_MIDDLE:  // do stop/pause
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					F_stop=true;
					//Robot.pausePlayFile();
					if(nMusicRedy == 255)
						Robot.pausePlayFile();
					//Robot.leds_Stop();
					oldKey=key;
					return;
				}
				break;
		}// end switch(key)
		oldKey=key;
		//chage modes, if buttons PS2 are pressed
		Robot.getPS2ControlInput();
		switch(PS_PAD_key){
			case BUTTON_MIDDLE:  // do stop/pause
				PS_PAD_key= -1;
				F_stop=true;
				//Robot.pausePlayFile();
				if(nMusicRedy == 255)
					Robot.pausePlayFile();
				//Robot.leds_Stop();
				return;
			case BUTTON_UP:  // change mode
				PS_PAD_key= -1;
				changeMode(-1);
				return;
			case BUTTON_DOWN:  // change mode
				PS_PAD_key= -1;
				changeMode(1);
				return;
		} // end switch(PS_PAD_key)
		textManager.printTimeTempC(15,12,0);
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		delay(20);
	}// end while(1)
}



// Change the control mode and display it on the LCD
void changeMode(int changeDir){

  // alternate modes
  modeBR += changeDir;
  if(modeBR < 0){ 
    modeBR=1;
  }
  else if(modeBR > 1)
    modeBR=0;
    
  // display  the mode on screen  
  Robot.fill(255, 255, 255);  
  Robot.stroke(255, 255, 255);
  Robot.rect(0,18,128,14);
  Robot.stroke(0, 0, 0);
  textManager.writeText_P(PSTR("Control:"),2,1);
  textManager.writeScriptPGM(scripts_ModeName,modeBR,2,10);
  WaitAnyTime(2000,0); // delay(2000);
}





//--------------------------------------------------------------------------------------
void Move(void){

	stateMove=INIT_FOR_MOVE;

	while (true){

		Robot.getPS2ControlInput();
		switch (stateMove){
			case INIT_FOR_MOVE:
				Serial.println(F("==IFM"));
				textManager.HeadScreenMove();
				resetFlags();
				resetFlags1();
				//initFlagsForAnyState();
				F_powerOn=false;
				F_manual=false;
				F_blockJoystick=false;
				prTurn=45;
				speedML=180;
				speedMR=180;
				stateMove=POWER_STATE_OFF;
				//old_stateMove=INIT_FOR_MOVE;
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
				}
				return;

			case STOP_STATE:
				if(F_printState){
					Serial.println(F("==STOP"));
					if(F_displSmilesEn){
						Robot.fill(WHITE);  
						Robot.stroke(WHITE);
						Robot.rect(0,68,128,80);
						Robot.stroke(BLACK);
						Robot._drawBMP_P(PSTR("Smile3m.bmp"),26,68);
					}
					Robot.fill(WHITE);  
					Robot.stroke(WHITE);
					Robot.rect(0,18,128,24);
					Robot.stroke(BLACK);
					Robot.setTextSize(2);
					textManager.writeText_P(PSTR("STOP"),1,1);
					Robot.setTextSize(1);
					cDisplayState_5s=0;
					F_displayState=true;
					if(F_displSmilesEn)
						Init_PS2();
				}
				break;

			case POWER_STATE_OFF:
				if(F_printState){
					Serial.println(F("==PWOF"));
					if(F_displSmilesEn){
						Robot.fill(WHITE);  
						Robot.stroke(WHITE);
						Robot.rect(0,68,128,80);
						Robot.stroke(BLACK);
						Robot._drawBMP_P(PSTR("Smile4m.bmp"),26,68);
					}
					Robot.fill(WHITE);  
					Robot.stroke(WHITE);
					Robot.rect(0,18,128,24);
					Robot.stroke(BLACK);
					Robot.setTextSize(2);
					textManager.writeText_P(PSTR("STAND BY"),1,1);
					Robot.setTextSize(1);
					cDisplayState_5s=0;
					F_displayState=true;
					Init_PS2();
				}
			 #ifdef OPT_TERMINAL_MONITOR  
				TerminalMonitor();           
			 #endif
				break;

			case TURN_TO_OPENING:
				if(F_printState){
					Serial.println(F("==TO"));
					if(F_displSmilesEn){
						Robot.fill(WHITE);  
						Robot.stroke(WHITE);
						Robot.rect(0,68,128,80);
						Robot.stroke(BLACK);
						Robot._drawBMP_P(PSTR("Smile7m.bmp"),26,68);
					}
					Robot.fill(WHITE);  
					Robot.stroke(WHITE);
					Robot.rect(0,18,128,24);
					Robot.stroke(BLACK);
					Robot.setTextSize(2);
					textManager.writeText_P(PSTR("RUN!"),1,1);
					Robot.setTextSize(1);
					cDisplayState_5s=0;
					F_displayState=true;
					if(F_displSmilesEn)
						Init_PS2();
					if(!F_contFromStop){
						playChoicedMusicFile();
						Robot.leds_Show();
					}
				}

				if(F_forvard && !F_TurnToOpening && !F_wiskersAny)
					PrevCheckUS_Fr();		//set slow speed ifForvard < 40cm

				if(F_reverce && !F_TurnToOpening && !F_wiskersAny)
					PrevCheckUS_Rv();		//set slow speed ifReverce <40cm

				if(F_forvard && !F_wiskerFr){
					TurnToOpeningFr();
					if(F_stop){
						break;
					}
				}
				if(F_reverce && !F_wiskerRv){
					TurnToOpeningRv();
					if(F_stop){
						break;
					}
				}
				if(F_forvard){
					F_wiskerLR=false;
					F_wiskerRR=false;
					if(!F_wiskerRF){
						checkWiskerLF();
						if(F_stop){
							break;
						}
					}
					if(!F_wiskerLF){
						checkWiskerRF();
						if(F_stop){
							break;
						}
					}
				}
				else if(F_reverce){
					F_wiskerLF=false;
					F_wiskerRF=false;
					if(!F_wiskerRR){
						checkWiskerLR();
						if(F_stop){
							break;
						}
					}
					if(!F_wiskerLR){
						checkWiskerRR();
						if(F_stop){
							break;
						}
					}
				}
				F_wiskersAny=isWiskerAny();

				if(!F_reverce && !F_forvard){
				  #ifdef DEBUG
					if (F_DebugOutput){
						if (F_DebugRun) {
							Serial.println(F("==Fr0&f0"));
						}
					}
				  #endif
					if(!F_manual1)
						Robot.motorsWrite(speedML, speedMR);
					resetFlags();
					resetFlags1();
				}
				break; //end TURN_TO_OPENING
		} // end switch (stateMove)
		textManager.printTimeTempC(15,12,false);
		if(Robot.F_enableCheckLight_1s)
			Robot.checkLight();
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		timeSelector();
		keySelection();
		F_printState=setFlagPrintState();
		old_stateMove=stateMove;
		printParm();
		delay(10);
	}// and while(true)

}






//********************************************************************************************
void TurnToOpeningFr(){
	uint8_t DistanceFr;
#ifdef DEBUG
	if (F_DebugOutput) {
		Serial.println(F("==TOfr"));
	}
#endif
	F_wiskerLR=false;
	F_wiskerRR=false;

	if(US_BusyStatusFr == PrCheck_US_fr)		// US BUSY prev operation
		return;

	DistanceFr=Robot.UScheckDistanceFr();

#ifdef DEBUG
	if (F_DebugOutput) {
		if (F_DebugRun) {
			Serial.print(F("TODisFr=")); Serial.println(DistanceFr, DEC);
		}
	}
#endif

	if(DistanceFr == 0){						// US BUSY, time < 50 ms
		US_BusyStatusFr=TO_USfr;
		return;
	}
	else 
		US_BusyStatusFr=0;

	F_afterStopDelay=isStopDelayEnded();	// F_afterStopDelay=1 if 1c ended

	if(DistanceFr == 1){					// (< 30cm)
		if(!F_afterStopDelay && !F_USstop){
			Robot.motorsStop();				// stop the motors 
			//old_speedML=0; old_speedMR=0;
			F_USstop=true;
			F_TurnToOpening=true;
			F_slowFr=false;
			old_stopDelay=millis();			 // start delay 
			return;
		}
		else if (F_afterStopDelay){
			if(!F_manual){
				Robot.turn(prTurn);
				old_minTurn=61;
			}
			resetFlags();
			F_TurnToOpening=false;
		}
	}
	else if(((DistanceFr > 2) && !F_slowFr) || F_slowRv){ // (>= 30cm)
#ifdef DEBUG
		if (F_DebugOutput) {
			if (F_DebugRun) {
				Serial.println(F("==>30fr"));
			}
		}
#endif
		if(!F_manual1)
			Robot.motorsWrite(speedML, speedMR);
		resetFlags();
		F_TurnToOpening=false;
	}
}  


void TurnToOpeningRv(){
	uint8_t DistanceRv;
#ifdef DEBUG
	if (F_DebugOutput) {
		Serial.println(F("==TOrv"));
	}
#endif
	F_wiskerLF=false;
	F_wiskerRF=false;

	if(US_BusyStatusRv == PrCheck_US_rv)						// US BUSY prev operation
		return;

	DistanceRv=Robot.UScheckDistanceRv();

#ifdef DEBUG
	if (F_DebugOutput) {
		if (F_DebugRun) {
	Serial.print(F("TODisRv=")); Serial.println(DistanceRv, DEC);
		}
	}
#endif

	if(DistanceRv == 0){						// US BUSY, time < 50 ms
		US_BusyStatusRv=TO_USrv;
		return;
	}
	else 
		US_BusyStatusRv=0;

	F_afterStopDelay=isStopDelayEnded();	 // F_afterStopDelay=1 if 1c ended

	if(DistanceRv == 1){					 // <30cm
		if(!F_afterStopDelay && !F_USstop){
			Robot.motorsStop();				 // stop the motors 
			//old_speedML=0; old_speedMR=0;
			F_USstop=true;
			F_TurnToOpening=true;
			F_slowRv=false;
			old_stopDelay=millis();			 // start delay 
			return;
		}
		else if (F_afterStopDelay){
			if(!F_manual){
				Robot.turn(prTurn);
				old_minTurn=61;
			}
			resetFlags();
			F_TurnToOpening=false;
		}
	}
	else if(((DistanceRv > 2) && !F_slowRv) || F_slowFr){ // (>= 30cm)
		#ifdef DEBUG
		if (F_DebugOutput) {
			if (F_DebugRun) {
				Serial.println(F("==>30rv"));
			}
		}
		#endif
		if(!F_manual1)
			Robot.motorsWrite(speedML, speedMR);
		resetFlags();
		F_TurnToOpening=false;
	}
}  


void PrevCheckUS_Fr(){
	int speedML_t;
	int speedMR_t;
	uint8_t DistanceFr;

#ifdef DEBUG
	if (F_DebugOutput) {
		Serial.println(F("==PrUSFR"));
	}
#endif
	if(((millis()-old_slowDelay) > 500) && F_slowFr){
		F_slowFr=false;
	}

	if(US_BusyStatusFr == TO_USfr)						// US BUSY prev operation
		return;

	DistanceFr=Robot.UScheckDistanceFr();

#ifdef DEBUG
	if (F_DebugOutput) {
		if (F_DebugRun) {
			Serial.print(F("PrDisFr=")); Serial.println(DistanceFr, DEC);
		}
	}
#endif

	if(DistanceFr == 0){								// US BUSY, time < 50 ms
		US_BusyStatusFr=PrCheck_US_fr;
		return;
	}
	else
		US_BusyStatusFr=0;

	if((DistanceFr == 1 || DistanceFr == 2) && !F_slowFr){	// (< 40)
		speedML_t=speedML;
		speedMR_t=speedMR;
		if(speedML > 80)
			speedML_t= 80;
		if(speedMR > 80)
			speedMR_t= 80;
		Robot.motorsWrite(speedML_t, speedMR_t);
		F_slowFr=true;
		F_slowRv=false;
#ifdef DEBUG
		if (F_DebugOutput) {
			Serial.println(F("=slFr"));
		}
#endif
		old_slowDelay=millis();					// start delay 
	}
	else if((DistanceFr > 2) && F_slowFr){		// (>= 40)
		F_slowFr=false;
		F_slowRv=false;
	}
}


void PrevCheckUS_Rv(){
	int speedML_t;
	int speedMR_t;
	uint8_t DistanceRv;

#ifdef DEBUG
	if (F_DebugOutput) {
		Serial.println(F("==PrUSRV"));
	}
#endif
	if(((millis()-old_slowDelay) > 500) && F_slowRv){
		F_slowRv=false;
	}

	if(US_BusyStatusRv == TO_USrv)						// US BUSY prev operation
		return;

	DistanceRv=Robot.UScheckDistanceRv();
#ifdef DEBUG
	if (F_DebugOutput) {
		if (F_DebugRun) {
			Serial.print(F("PrDisRv=")); Serial.println(DistanceRv, DEC);
		}
	}
#endif

	if(DistanceRv == 0){								// US BUSY, time < 50 ms
		US_BusyStatusRv=PrCheck_US_rv;
		return;
	}
	else
		US_BusyStatusRv=0;

	if((DistanceRv == 1 || DistanceRv == 2) && !F_slowRv){	// (< 40)
		speedML_t=speedML;
		speedMR_t=speedMR;
		if(speedML < -80)
			speedML_t= -80;
		if(speedMR < -80)
			speedMR_t= -80;
		Robot.motorsWrite(speedML_t, speedMR_t);
		F_slowRv=true;
		F_slowFr=false;
#ifdef DEBUG
		if (F_DebugOutput) {
			Serial.println(F("=slRv"));
		}
#endif
		old_slowDelay=millis();					// start delay 
	}
	else if((DistanceRv > 2) && F_slowRv){		// (>= 40)
		F_slowRv=false;
		F_slowFr=false;
	}
}


void checkWiskerLF(){
	uint8_t lw;

	//Serial.println("==LFW");

	F_wiskerLR=false;
	F_wiskerRR=false;

	F_afterStopDelay=isStopDelayEnded(); // F_afterStopDelay=1 if 1c ended

	if(lw=Robot.getWiskerLF() != 0){ // 10cm
		if((!F_afterStopDelay) && (!F_USstop) && (lw == 2)){
			Robot.motorsStop(); // stop the motors 
			//old_speedML=0; old_speedMR=0;
			F_USstop=true;
			F_wiskerLF=true;
			old_stopDelay=millis(); // start delay 
			return;
		}
		else if (F_afterStopDelay){
			if(!F_manual){
				Robot.turn(90);
			}
			resetFlags();
			F_wiskerLF=false;
			//F_afterStopDelay=false;
			//F_USstop=false;
			//F_slowFr=false;
			//F_slowRv=false;
		}
	}
	else if(lw == 0 && !F_TurnToOpening){ //Robot.getLeftWisker() >10cm
		resetFlags();
		F_wiskerLF=false;
		//F_afterStopDelay=false;
		//F_USstop=false;
		//F_slowFr=false;
		//F_slowRv=false;
	}
}


void checkWiskerRF(){
	bool rw;

	//Serial.println("==RFW");

	F_wiskerLR=false;
	F_wiskerRR=false;

	F_afterStopDelay=isStopDelayEnded(); // F_afterStopDelay=1 if 1c ended

	if(rw=Robot.getWiskerRF() != 0){ // 10cm
		if((!F_afterStopDelay) && (!F_USstop) && (rw == 2)){
			Robot.motorsStop(); // stop the motors 
			//old_speedML=0; old_speedMR=0;
			F_USstop=true;
			F_wiskerRF=true;
			old_stopDelay=millis(); // start delay 
			return;
		}
		else if (F_afterStopDelay){
			if(!F_manual){
				Robot.turn(-90);
			}
			resetFlags();
			F_wiskerRF=false;
			//F_afterStopDelay=false;
			//F_USstop=false;
			//F_slowFr=false;
			//F_slowRv=false;
		}
	}
	else if(!rw && !F_TurnToOpening){ //Robot.getRightWisker() >10cm
		resetFlags();
		F_wiskerRF=false;
		//F_afterStopDelay=false;
		//F_USstop=false;
		//F_slowFr=false;
		//F_slowRv=false;
	}
}


void checkWiskerLR(){
	bool lw;

	//Serial.println("==LRW");

	F_wiskerLF=false;
	F_wiskerRF=false;

	F_afterStopDelay=isStopDelayEnded();

	if(lw=Robot.getWiskerLR() != 0){ // 10cm
		if((!F_afterStopDelay) && (!F_USstop) && (lw == 2)){
			Robot.motorsStop(); // stop the motors 
			//old_speedML=0; old_speedMR=0;
			F_USstop=true;
			F_wiskerLR=true;
			old_stopDelay=millis(); // start delay 
			return;
		}
		else if (F_afterStopDelay){
			if(!F_manual){
				Robot.turn(-90);
			}
			resetFlags();
			F_wiskerLR=false;
			//F_afterStopDelay=false;
			//F_USstop=false;
			//F_slowFr=false;
			//F_slowRv=false;
		}
	}
	else if(!lw && !F_TurnToOpening){ //Robot.getLeftWisker() >10cm
		resetFlags();
		F_wiskerLR=false;
		//F_afterStopDelay=false;
		//F_USstop=false;
		//F_slowFr=false;
		//F_slowRv=false;
	}
}


void checkWiskerRR(){
	bool rw;

	//Serial.println("==RRW");

	F_afterStopDelay=isStopDelayEnded();

	F_wiskerLF=false;
	F_wiskerRF=false;

	if(rw=Robot.getWiskerRR() != 0){ // 10cm
		if((!F_afterStopDelay) && (!F_USstop) && (rw == 2)){
			Robot.motorsStop(); // stop the motors 
			//old_speedML=0; old_speedMR=0;
			F_USstop=true;
			F_wiskerRR=true;
			old_stopDelay=millis(); // start delay 
			return;
		}
		else if (F_afterStopDelay){
			if(!F_manual){
				Robot.turn(90);
			}
			resetFlags();
			F_wiskerRR=false;
			//F_afterStopDelay=false;
			//F_USstop=false;
			//F_slowFr=false;
			//F_slowRv=false;
		}
	}
	else if(!rw && !F_TurnToOpening){ //Robot.getRightWisker() >10cm
		resetFlags();
		F_wiskerRR=false;
		//F_afterStopDelay=false;
		//F_USstop=false;
		//F_slowFr=false;
		//F_slowRv=false;
	}
}


void resetFlags(){
	F_afterStopDelay=false;
	F_USstop=false;
	F_slowFr=false;
	F_slowRv=false;
}


void resetFlags1(){
	F_TurnToOpening=false;
	F_wiskerLR=false;
	F_wiskerRR=false;
	F_wiskerLF=false;
	F_wiskerRF=false;
}


bool isWiskerFr(){
	if(F_wiskerLF || F_wiskerRF)
		F_wiskerFr=true;
	else
		F_wiskerFr=false;
	return F_wiskerFr;
}


bool isWiskerRv(){
	if(F_wiskerLR || F_wiskerRR)
		F_wiskerRv=true;
	else
		F_wiskerRv=false;
	return F_wiskerRv;
}


bool isWiskerAny(){
	bool fr,rv, fwiskAny;

	fr=isWiskerFr();
	rv=isWiskerRv();
	if(fr || rv)
		fwiskAny=true;
	else
		fwiskAny=false;
#ifdef DEBUG
	if (F_DebugOutput){
		if (F_DebugWiskers){
			Serial.print(F("F_wkLF= "));
			Serial.print(F_wiskerLF);
			Serial.print(F(", F_wkLR= "));
			Serial.print(F_wiskerLR);
			Serial.print(F(", F_wkRF= "));
			Serial.print(F_wiskerRF);
			Serial.print(F(", F_wkRR= "));
			Serial.print(F_wiskerRR);
			Serial.print(F(", F_wiskersAny= "));
			Serial.println(fwiskAny);
		}
	}  
#endif
	return fwiskAny;
}


bool isStopDelayEnded(){
	bool f_afterStopDel;

	if(((millis()-old_stopDelay) > 500) && F_USstop){
		f_afterStopDel=true;
	}
	else
		f_afterStopDel=false;

	return f_afterStopDel;
}


void InitFromStop(void){
}  


void timeSelector(){
	
	textManager.updateTime(false);

	if (old_minTurn != textManager.minute){
		c_minuteTurn++;
		if(c_minuteTurn == 2){
			prTurn= -prTurn;
			c_minuteTurn=0;
		}
		old_minTurn=textManager.minute;
	}
	if (old_second != textManager.second){
		if(F_displayState){
			cDisplayState_5s++;
			if(cDisplayState_5s == 5){
				Robot.fill(WHITE);  
				Robot.stroke(WHITE);
				Robot.rect(0,18,128,24); // erase text
				Robot.stroke(BLACK);
				cDisplayState_5s=0;
				F_displayState=false;
			}
		}
		if(F_displ_ErrorVolt){
			cDisplayErrorVolt_15s++;
			if(cDisplayErrorVolt_15s == 15){
				Robot.fill(WHITE);  
				Robot.stroke(WHITE);
				Robot.rect(0,40,128,40); // erase text
				Robot.stroke(BLACK);
				cDisplayErrorVolt_15s=0;
				F_displ_ErrorVolt=false;
			}
		}
	}
		old_second=textManager.second;
}
















//-----------------------------------------------------------------------------------
void serviceMenu(){

	initFlagsForAnyState();
	Robot.FrontLightOnOff(false);
	F_enableLightAuto=false;

	PosServiceMenu=textManager.inputServiceMenu(3,2,PosServiceMenu);

	switch(PosServiceMenu){
		case SET_USER_DATA:
			SetUserData();
			break;
		case GET_INFO:
			GetInfo();
			break;
		case SET_TIME:
			textManager.getTimeSelection(4,7);
			break;
		case WHEEL_CALIBRATION:
			wheelCalibration();
			break;
		case CHOICE_MUSIC:
			ChoiceMusicMenu();
			break;
		case SET_MAX_PIC_COLL:
			textManager.getMaxPicCollSelect(4,13);
			break;
	}
}




void SetUserData(){

	//initFlagsForAnyState();

	textManager.writeScriptPGM(scripts_User,0,1,1);
	textManager.inputUser(3,1,USERNAME);

	textManager.fillWindowLineCol(0,4,0,20);

	Robot.userNameRead(buffer);
	textManager.writeText_P(PSTR("Hello"),1,2);
	textManager.writeText(1, 8, buffer);
	textManager.writeScriptPGM(scripts_User,1, 3, 1);
	textManager.inputUser(5, 1, ROBOTNAME);

	textManager.fillWindowLineCol(3,6,0,20);

	textManager.writeScriptPGM(scripts_User,2,3,1);
	textManager.inputUser(5,1,CITYNAME);

	textManager.fillWindowLineCol(0,8,0,20);

	textManager.writeScriptPGM(scripts_User,3,1,1);
	textManager.inputUser(3,1,COUNTRYNAME);

	Robot.clearScreen();

	Robot.userNameRead(buffer);
	textManager.writeText_P(PSTR("Very well!"),1,2);
	textManager.writeText(2,3,buffer);
	textManager.writeScriptPGM(scripts_User,12, 4,1);

	Robot.waitContinueKey();
	Robot.clearScreen();
}




void GetInfo(){

	//initFlagsForAnyState();

	Robot.userNameRead(buffer);
	textManager.writeText_P(PSTR("UserID:  "),1,2);
	textManager.writeText(1, 12, buffer);

	Robot.robotNameRead(buffer);
	textManager.writeText_P(PSTR("RobotID: "),3,2);
	textManager.writeText(3, 12, buffer);

	Robot.cityNameRead(buffer);
	textManager.writeText_P(PSTR("City:    "),5,2);
	textManager.writeText(5, 12, buffer);

	Robot.countryNameRead(buffer);
	textManager.writeText_P(PSTR("Country: "),7,2);
	textManager.writeText(7, 12, buffer);

	Robot.waitContinueKey();
	Robot.clearScreen();
}





//-------------------------------------------------------------------------------
//initialization flags for stateMove=ANY_STATE
//-------------------------------------------------------------------------------
void initFlagsForAnyState(){

	F_powerOn=true;
	F_stop=false;
	F_blockContinue=false;
	F_contFromStop=false;
	F_ChoiceMusic=false;
	F_DanceBot=false;
	F_enableLightAuto=true;
	F_skeepWaitAnyTime=false;
	F_blockJoystick=true;
	stateMove=ANY_STATE;
}



//------------------------------------------------------------------------------
void turnARobotOff(){		// Turn off

	Robot.motorsStop(); // stop the motors
	//old_speedML=0; old_speedMR=0;
	resetFlags();
	resetFlags1();
	stateMove=POWER_STATE_OFF;
}
  
//------------------------------------------------------------------------------
bool setFlagPrintState(){
	if(stateMove != old_stateMove)
		return true;
	else
		return false;
}


//------------------------------------------------------------------------------
void printParm(){

}


//------------------------------------------------------------------------------
void Init_PS2(){

	do {
	PS2_Error=Robot.PS2ControlInit();
	if(!PS2_Error) break;
	delay(50);
	}while (PS2_Error);			// do it if PS2_Error != 0
}


//------------------------------------------------------------------------------
void playChoicedMusicFile(){

#ifdef DEBUG
	nMusic=Robot.readByte(ADDR_CHOICE_MUSIC);
#endif
	printReadFromEE_track(nMusic);

	getFaleNameFromScriptPGM(scripts_FaleNameMusic,nMusic,FaleExt_wav_scr);
	Robot.playFile(fileName,IGNORE_isPlAY);
	nMusicRedy=255;
}



//--------------------------------------------------------------------------------------------------
#ifdef DEBUG
void printReadFromEE_track(uint8_t track){

	Serial.print(F("ReadFromEE_track: "));
	Serial.println(track, DEC);
}
#endif



//--------------------------------------------------------------------------------------------------
void getFaleNameFromScriptPGM(const char* const* FaleName_script_P, uint8_t nStr, const char* FaleNameExt_scripts_P){

	strcpy_P(fileName, (char*)pgm_read_word(&(FaleName_script_P[nStr])));	//"Macarena" --> fileName
	strcpy_P(buffer1, (char*)(FaleNameExt_scripts_P));						// ".wav" --> buffer1
	strcat(fileName,buffer1);												// fileName+buffer1 = Macarena+.waw= Macarena.waw
//#ifdef DEBUG
//	Serial.println(fileName);
//#endif
}



void keySelection(){
	int8_t key;

	switch(key=Robot.keyboardRead()){
		case BUTTON_UP:
			if(key != oldKey){
				Robot.playSoundBuzz(7);
			}
			break;
		case BUTTON_DOWN:
			if(key != oldKey){
				Robot.playSoundBuzz(7);
			}
			break;
		case BUTTON_LEFT:
			if(key != oldKey){
				Robot.playSoundBuzz(7);
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
					Robot.leds_Stop();
					Robot.playFileDisable();
					stateMove=RETURN_STATE;
				}
				else if(stateMove == ANY_STATE){
					if(F_stop){
						Robot.stopPlayFile();
						Robot.playFileDisable();
						Robot.leds_Stop();
						F_blockContinue=false;
						F_powerOn=false;
						F_stop=false;
					}
				}
				F_ShowDebugPrompt=true;
			}
			break;
		case BUTTON_RIGHT:
			if(key != oldKey){
				Robot.playSoundBuzz(7);
				if(stateMove == POWER_STATE_OFF){
					curSeq=0;
					F_contFromStop=false;
					stateMove=TURN_TO_OPENING; // or RUN_DANCE
				}
				else if(stateMove == ANY_STATE){
					F_skeepWaitAnyTime=true;
				}
				F_powerOn=true;
			}
			break;
		case BUTTON_MIDDLE:
			if(key != oldKey){
				Robot.playSoundBuzz(7);
				if(stateMove == TURN_TO_OPENING){ // or RUN_DANCE
					F_stop = true;
					Robot.motorsStop();
					//old_speedML=0; old_speedMR=0;
					Robot.FrontLightOnOff(false);
					if(nMusicRedy == 255)
						Robot.pausePlayFile();
					//Robot.leds_Stop();
					Robot.leds_Pause_Show();
					resetFlags();
					resetFlags1();
					stateMove=STOP_STATE;
				}
				else if(stateMove == STOP_STATE){
					F_stop=false;
					if(nMusicRedy == 255)
						Robot.pausePlayFile();
					Robot.leds_Pause_Show();
					F_contFromStop=true;
					stateMove=TURN_TO_OPENING; // or RUN_DANCE
				}
				else if(stateMove == ANY_STATE){
					if(!F_blockContinue){
						F_stop=!F_stop;
						if(nMusicRedy == 255)
							Robot.pausePlayFile();
						Robot.leds_Pause_Show();
						F_contFromStop=true;
						if(F_stop){
							Robot.motorsStop();
							//Robot.leds_Stop();
						}
					}
				}
			}
			break;
	}
	oldKey=key;
}












//-------------------------------------------------------------------------------
// TerminalMonitor - us to do anything, like update debug levels ore the like.
//-------------------------------------------------------------------------------
#ifdef OPT_TERMINAL_MONITOR

bool TerminalMonitor(void){
	byte szCmdLine[20];  // 
	byte ich;
	int  ch;

	if (F_ShowDebugPrompt) {
		DBGSerial.println(F("A-Robot Monitor:"));
		DBGSerial.println(F("D - Toggle debug On or Off"));

		F_ShowDebugPrompt=false;
	}

	if ((ich=DBGSerial.available())) {
		ich = 0;
		// For now assume we receive a packet of data from serial monitor, as the user has
		// to click the send button...
		for (ich=0; ich < sizeof(szCmdLine); ich++) {
			ch = DBGSerial.read();        // get the next character
			if ((ch == -1) || ((ch >= 10) && (ch <= 15)))
				break;
			szCmdLine[ich] = ch;
		}
		szCmdLine[ich] = '\0';    // go ahead and null terminate it...
		DBGSerial.print(F("Serial Cmd Line:"));        
		DBGSerial.write(szCmdLine, ich);
		DBGSerial.println(F("!!"));

		if (ich == 0) {
			F_ShowDebugPrompt = true;
		} 
		else if ((ich == 1) && ((szCmdLine[0] == 'd') || (szCmdLine[0] == 'D'))) {
			F_DebugOutput = !F_DebugOutput;
			if (F_DebugOutput) 
				DBGSerial.println(F("Debug is on"));
			else
				DBGSerial.println(F("Debug is off"));
		} 
		else if ((ich == 2) && ((szCmdLine[0] == 'd') || (szCmdLine[0] == 'D'))) {
			switch (szCmdLine[1]){
			case '1': 
				break;
			case '2': 
				F_DebugVoltage=!F_DebugVoltage;
				break;
			case '3': 
				F_DebugRun=!F_DebugRun;
				break;
			case '4': 
				F_DebugWiskersTest=!F_DebugWiskersTest;
				break;
			case '5': 
				F_DebugWiskers=!F_DebugWiskers;
				break;
			case '6': 
				F_DebugVlight=!F_DebugVlight;
				break;
			case '7': 
				F_UDistance=!F_UDistance;
				break;
			case '8': 
				F_DebugPS2Input=!F_DebugPS2Input;
				break;
			case '9': 
				F_LxLyRxRy=!F_LxLyRxRy;
				break;
			case 'a': 
				F_DebugDance=!F_DebugDance;
				break;
			case 'b': 
				break;
			case 'B': 
				break;
			case 'c': 
				break;
			case 'D': 
				break;
			case 't': 
				break;
			case 'r': 
				break;
			case 'u': 
				break;
			case 's': 
				break;
			case 'p': 
				break;
			case 'g': 
				break;
			case 'G': 
				break;
			} 
		}
		return true;
	} // end, (ich = DBGSerial.available())
	return false;
}
#endif //OPT_TERMINAL_MONITOR







