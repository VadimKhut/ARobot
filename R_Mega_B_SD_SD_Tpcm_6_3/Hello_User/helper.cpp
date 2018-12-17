#include "ArduinoRobot.h"
#include "RobotTextManager.h"



extern void Init_PS2();
extern void WaitAnyTime(uint16_t wait_time, bool block);
extern void keySelection(void);


extern int8_t oldKey;
extern int8_t PS_PAD_key;
extern bool F_powerOn;
extern bool F_skeepWaitAnyTime;
extern bool F_contFromStop;
extern bool F_stop;





void RobotControl::drawBase(){
	Arduino_LCD::drawCircle(64,80,50,foreGround);
	Arduino_LCD::drawLine(64,30,64,20,foreGround);
}


void RobotControl::drawDire(int16_t dire){
	static uint8_t x_old;
	static uint8_t y_old;
	static uint8_t x_t_old;
	static uint8_t y_t_old;

	uint8_t x=60*sin(dire/360.0*6.28)+64;
	uint8_t x_t=40*sin(dire/360.0*6.28)+64;
	uint8_t y=60*cos(dire/360.0*6.28)+80;
	uint8_t y_t=40*cos(dire/360.0*6.28)+80;

	Arduino_LCD::drawLine(x_t_old,y_t_old,x_old,y_old,backGround);
	Arduino_LCD::drawLine(x_t,y_t,x,y,RED);

	x_old=x;
	y_old=y;
	x_t_old=x_t;
	y_t_old=y_t;
}


void RobotControl::drawCompass(uint16_t value, uint8_t x, uint8_t y){
	drawBase();
	drawDire(value);
	debugPrint(value,x,y); // x=57; y=76
}


//display logos 
void RobotControl::displayLogos(){

	playFile_P(PSTR("ORGEL.wav"),IGNORE_isPlAY); 

	_drawBMP_P(PSTR("lg0.bmp"),0,0);
	leds_Show(COLOR_WIPE_RGBW_UP,1,1,50);

	textManager.HeadScreenDance();
	WaitAnyTime(2500,0); // delay(2500);
	if(!F_powerOn){
		clearScreen();
		leds_Stop();
		return;
	}

	_drawBMP_P(PSTR("lg1.bmp"),0,0);
	leds_Show(COLOR_WIPE_RGBW_DOUN,20,1,50);

	textManager.HeadScreenDance();
	WaitAnyTime(2500,0); // delay(2500);
	if(!F_powerOn){
		clearScreen();
		leds_Stop();
		return;
	}
	
	while(::digitalRead(TMP_REDY) || F_stop){
		keySelection();
		getPS2ControlInput();
		if(!F_powerOn)
			break;
		if(F_skeepWaitAnyTime){		// BUTTON_RIGHT or PAD_RIGHT
			F_skeepWaitAnyTime=false;
			F_stop=false;
			break;
		}
		if(F_stop && F_contFromStop){ //pause
			F_contFromStop=false;
		}
		else if(!F_stop && F_contFromStop){ //continue
			F_contFromStop=false;
		}
		textManager.printTimeTempC(15,12,false);
		if(F_enableCheckLight_1s)
			checkLight();
		if(F_enableCheckVBat_1s)
			checkVoltage();
		delay(20);
	} //end while()
	if(!F_powerOn){
		clearScreen();
		leds_Stop();
		return;
	}
	clearScreen();
	leds_Stop();
	stopPlayFile();
	playFileDisable();
}


//wait for for the desired button to be pressed
void RobotControl::waitContinueKey(uint8_t key){
	int8_t rkey;
	PS_PAD_key= -1;

	Init_PS2();

	while(true){
		rkey=keyboardRead();
		if(rkey != oldKey){
			if(rkey == key){
				playSoundBuzz(7);
				oldKey=rkey;
				break;
			}
		}
		if(rkey == BUTTON_NONE)
			oldKey=rkey;

		getPS2ControlInput();
		if(PS_PAD_key == key){
			break;
		}
		delay(10);
	}
}


//wait for the desired button key or PS2 PAD to be pressed and display Time
void RobotControl::waitContinueKey_T(uint8_t key){
	int8_t rkey;
	PS_PAD_key= -1;

	Init_PS2();

	while(true){
		rkey=keyboardRead();
		if(rkey != oldKey){
			if(rkey == key){
				playSoundBuzz(7);
				oldKey=rkey;
				break;
			}
		}
		if(rkey == BUTTON_NONE)
			oldKey=rkey;

		getPS2ControlInput();
		if(PS_PAD_key == key){
			break;
		}

		textManager.printTimeTempC(15,12,false);
		if(F_enableCheckLight_1s)
			checkLight();
		if(F_enableCheckVBat_1s)
			checkVoltage();
		delay(10);
	}
}



//wait for a ANY key or PS2 PAD button to be pressed and display Time
uint8_t RobotControl::waitContinueAnyKey_T(){
	int8_t key;
	//PS_PAD_key= -1;

	Init_PS2();

	while(true){
		key=keyboardRead();
		if(key != oldKey){
			if(key != BUTTON_NONE){
				playSoundBuzz(7);
				oldKey=key;
				return key;
			}
		}
		if(key == BUTTON_NONE)
			oldKey=key;

		getPS2ControlInput();
		if(PS_PAD_key != -1)
			return PS_PAD_key;

		textManager.printTimeTempC(15,12,false);
		if(F_enableCheckLight_1s)
			checkLight();
		if(F_enableCheckVBat_1s)
			checkVoltage();
		delay(10);
	}
}
