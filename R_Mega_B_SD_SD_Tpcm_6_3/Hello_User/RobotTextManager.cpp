//#include <avr/pgmspace.h>
#include "ArduinoRobot.h"
#include "VirtualKeyboard.h"
#include "RobotTextManager.h"
//#include "OneWire.h"
#include "DS1307.h"
#include "scripts_Hello_User.h"

//*********************************************************
// display 128 x 160
// Max Col = 128/6 = 0, 20, Max Lin = 160/10 = 0, 15 
// When text size is 1, one character is 5*7
// lineHeight=8+2=10
// charWidth=5+1=6
//*********************************************************

//************************************************************************************
const int TextManager::lineHeight=10;
const int TextManager::charWidth=6;
bool TextManager::F_convertDelay=false;
bool TextManager::F_enableConvertC_1s=false;
bool RobotControl::F_enableCheckLight_1s=false;
bool RobotControl::F_enableCheckVBat_1s=false;


extern void getFaleNameFromScriptPGM(const char* const* FaleName_script_P, uint8_t nStr, const char* FaleNameExt_scripts_P);
extern void initFlagsForAnyState(void);

extern int oldKey;
extern bool F_DebugOutput;
extern bool F_DebugTemperatC;
extern int8_t PS_PAD_key;
extern void Init_PS2();
extern char fileName[13];
extern bool F_ChoiceMusic;


//bool F_testWiskers_1s=false;
void testAllWisker();
//************************************************************************************


TextManager::TextManager(){
	//init for string buffers
	strcpy(time, "00:00:00");
	strcpy(date, "MON 00-00-2000");
	strcpy(temper, "23.0 *C");
	strcpy(PGMbuffer, "                 ");
}

void TextManager::setMargin(int margin_left,int margin_top){

	this->margin_left=margin_left;
	this->margin_top=margin_top;
}
int TextManager::getLine(int line){
		
	uint8_t s=Robot.getTextSize();
	return line*lineHeight*s+margin_top;
}

int TextManager::getCol(int col){

	uint8_t s=Robot.getTextSize();
	return col*charWidth*s+margin_left;
}

void TextManager::writeText(int line, int col, const char* txt, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(getCol(col),getLine(line));
	Robot.print(txt);
  
	Robot.setTextColor(BLACK);
}



void TextManager::writeTextNch(int line, int col, char* txt, uint8_t nchar, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(getCol(col),getLine(line));
	for (uint8_t i=0; i < nchar; i++)
		Robot.print(txt[i]);
  
	Robot.setTextColor(BLACK);
}


void TextManager::writeTextNchYX(int y, int x, char* txt, uint8_t nchar, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(x,y);
	for (uint8_t i=0; i < nchar; i++)
		Robot.print(txt[i]);
  
	Robot.setTextColor(BLACK);
}


void TextManager::writeText(int line, int col, char value, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(getCol(col),getLine(line));
	Robot.print(value);
  
	Robot.setTextColor(BLACK);
}

void TextManager::writeText(int line, int col, int value, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(getCol(col),getLine(line));
	Robot.print(value);
  
	Robot.setTextColor(BLACK);
}

void TextManager::writeText(int line, int col, long value, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(getCol(col),getLine(line));
	Robot.print(value);
  
	Robot.setTextColor(BLACK);
}

void TextManager::writeText(int line, int col, float valueFl, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(getCol(col),getLine(line));
	Robot.print(valueFl);
  
	Robot.setTextColor(BLACK);
}

void TextManager::writeTextYX(int y, int x, float valueFl, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(x,y);
	Robot.print(valueFl);
  
	Robot.setTextColor(BLACK);
}

void TextManager::debugPrint(const char* str, int line, int col){

	writeText(line,col,PGMbuffer,false);
	writeText(line,col,str,true);
	strcpy(PGMbuffer, str);	// save old str to PGMbuffer[]
}

void TextManager::debugPrint(char value, int line, int col){

	writeText(line,col,(char)oldValue,false);
	writeText(line,col,value,true);
	oldValue=value;
}

void TextManager::debugPrint(int value, int line, int col){

	if(value != oldValue){

		writeText(line,col,(int)oldValue,false);
		writeText(line,col,value,true);
		oldValue=value;
	}
}

void TextManager::debugPrint(long value, int line, int col){

	writeText(line,col,oldValue,false);
	writeText(line,col,value,true);
	oldValue=value;
}


void TextManager::debugPrint(float valueFl, int line, int col){

	writeText(line,col,old_valueFl,false);
	writeText(line,col,valueFl,true);
	old_valueFl=valueFl;
}


void TextManager::debugPrintYX(float valueFl, int y, int x){

	writeTextYX(y,x,old_valueFl,false);
	writeTextYX(y,x,valueFl,true);
	old_valueFl=valueFl;
}


//print a string from program space to a specific line,
//column on the LCD
void TextManager::writeScriptPGM(const char* const* scripts_PGM, int seq, int line, int col){

	//first fill the buffer with text from program space
	getPGMtextToBuffer(scripts_PGM,seq);
	//then print it to the screen
	textManager.writeText(line,col,PGMbuffer);
}


void TextManager::writeText_P(const char* scripts_P, int line, int col, bool onOff){

	//first fill the buffer with text from program space
	strcpy_P(PGMbuffer, scripts_P);
	//then print it to the screen
	textManager.writeText(line,col,PGMbuffer,onOff);
}


//It takes a string from program space, and fill it  
//in the buffer
void TextManager::getPGMtextToBuffer(const char* const* const scripts_P, int seq){

	strcpy_P(PGMbuffer, (char*)pgm_read_word(&(scripts_P[seq])));
}


void TextManager::fillWindowLineCol(int line, int nline, int col, int ncol, uint16_t c){
	int16_t x,y,w,h;

	if(nline == 0)
		nline=1;
	if(ncol == 0)
		ncol=1;

	x=getCol(col);
	y=getLine(line);
	w=getCol(ncol);
	h=getLine(nline);

	Robot.fillRect(x,y,w,h,c);
}


void TextManager::inputUser(int line,int col, int code){

	setInputPoolUser(code);
	getInputUser(line,col);
	pushInputUser(code);
}


void TextManager::drawInput(int line, int col, uint8_t wIn, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(getCol(col),getLine(line)+2);
	for( int i=0; i < wIn; i++){
		Robot.print('_');
	}
  
	Robot.setTextColor(BLACK);
}

void TextManager::mvUser(int dire){

	drawInput(inLin,inCol,1,false);

	if(dire < 0){
		if(inPos > 0){
			inPos--;
			inCol--;
		}
	}
	else{
		if(inPos < 16){
			inPos++;
			inCol++;
		}
	}
	drawInput(inLin,inCol,1,true);
}


void TextManager::refreshCurrentLetter(char letter){
  if(letter){
	writeText(inLin,inCol,inputPool+inPos,false);//erase
	inputPool[inPos]=letter;
	writeText(inLin,inCol,inputPool+inPos,true);//write
  }
}


void TextManager::getInputUser(int line, int col){

	writeText(line,col,">");  //print ">", Input indicator
  
	writeText(line, col+1, inputPool);
  
	inLin=line;  //Ini input cursor
	inCol=col+1;
	inPos=0;

	drawInput(inLin,inCol,1,true); // print('_');
  
	Vkey.display(100); // Vkey is a object of VirtualKeyboard class

	Init_PS2();
  
	while(true){
		switch(Robot.keyboardRead()){
			case BUTTON_LEFT:
				Robot.playSoundBuzz(7);
				mvUser(-1);
				break;
			case BUTTON_RIGHT:
				Robot.playSoundBuzz(7);
				mvUser(1);
				break;
			case BUTTON_MIDDLE:
				Robot.playSoundBuzz(7);
				char selection=Vkey.getSelection();
				if(selection != '\0'){
					refreshCurrentLetter(selection);
					mvUser(1);
				}
				else{ // selection == '\0')
					drawInput(inLin,inCol,1,false);
					return;
				}
		}
		Robot.getPS2ControlInput();
		Vkey.run();
		delay(10);
	}
}

void TextManager::setInputPoolUser(int code){

	switch(code){
		case USERNAME:
			Robot.userNameRead(inputPool);
			break;
		case ROBOTNAME:
			Robot.robotNameRead(inputPool);
			break;
		case CITYNAME:
			Robot.cityNameRead(inputPool);
			break;
		case COUNTRYNAME:
			Robot.countryNameRead(inputPool);
			break;
	}
	for(int i=0;i<18;i++){
		if(inputPool[i]=='\0'){
			for(int j=i; j < 18; j++){
				inputPool[j]='\0';
			}
			break;
		}
	}
}

void TextManager::pushInputUser(int code){

	switch(code){
		case USERNAME:
			Robot.userNameWrite(inputPool);
			break;
		case ROBOTNAME:
			Robot.robotNameWrite(inputPool);
			break;
		case CITYNAME:
			Robot.cityNameWrite(inputPool);
			break;
		case COUNTRYNAME:
			Robot.countryNameWrite(inputPool);
			break;
	}
	for(int i=0; i < 18; i++){
		inputPool[i]='\0';
	}
}

uint8_t TextManager::inputMainMenu(int line, int col, uint8_t pos){
	//uint8_t nMemu=7;

	line_s=line;
	col_s=col;

	HeadMainMenu(line,col);
	tMenu=MAIN_MENU;
	uint8_t selection=getMenuSelection(line,col-2,MAIN_MENU_LINE-1,pos);
	Robot.clearScreen();
	return selection;
}


uint8_t TextManager::inputDanceMenu(int line, int col, uint8_t pos){
	//uint8_t nMemu=7;

	line_s=line;
	col_s=col;

	HeadDanceMenu(line,col);
	tMenu=DANCE_MENU;
	uint8_t selection=getMenuSelection(line,col-2,DANCE_MENU_LINE-1,pos);
	Robot.clearScreen();
	return selection;
}


uint8_t TextManager::inputChoiceMusicMenu(int line, int col, uint8_t pos){
	//uint8_t nMemu=7;

	line_s=line;
	col_s=col;

	HeadChoiceMusicMenu(line,col);
	tMenu=MUSIC_MENU;
	uint8_t selection=getMenuSelection(line,col-2,DANCE_MENU_LINE-1,pos);
	Robot.clearScreen();
	return selection;
}


uint8_t TextManager::inputServiceMenu(int line, int col, uint8_t pos){
	//uint8_t nMemu=7;

	line_s=line;
	col_s=col;

	HeadServiceMenu(line,col);
	tMenu=SERVICE_MENU;
	uint8_t selection=getMenuSelection(line,col-2,SERVICE_MENU_LINE-1,pos);
	Robot.clearScreen();
	return selection;
}


void TextManager::HeadMainMenu(int line, int col){
	uint8_t nMemu=7;

	Robot.clearScreen();
	Robot.setTextSize(1);
  	textManager.writeText_P(PSTR("M E N U"),2,7); // print "MENU"
	Robot.drawRect(39,18,47,11,BLACK);	//(5+1)*7-1+2+2  charWidth=5, charMargin=1, count("Enter")=6, lastItem_MarginRight=0, marginLeft==marginRight=2
	textManager.printAnyMenu(scripts_Main_Menu,nMemu,line,col);
}


void TextManager::HeadChoiceMusicMenu(int line, int col){
	uint8_t nMemu=7;

	Robot.clearScreen();
	Robot.setTextSize(1);
  	textManager.writeText_P(PSTR("MUSIC"),2,7); // print "MUSIC"
	Robot.drawRect(39,18,35,11,BLACK);	//(5+1)*7-1+2+2  charWidth=5, charMargin=1, count("Enter")=6, lastItem_MarginRight=0, marginLeft==marginRight=2
	textManager.printAnyMenu(scripts_Choice_Music,nMemu,line,col);
}


void TextManager::HeadDanceMenu(int line, int col){
	uint8_t nMemu=7;

	Robot.clearScreen();
	Robot.setTextSize(1);
  	textManager.writeText_P(PSTR("DANCE"),2,7); // print "DANCE"
	Robot.drawRect(39,18,35,11,BLACK);	//(5+1)*7-1+2+2  charWidth=5, charMargin=1, count("Enter")=6, lastItem_MarginRight=0, marginLeft==marginRight=2
	textManager.printAnyMenu(scripts_Choice_Music,nMemu,line,col);
}


void TextManager::HeadServiceMenu(int line, int col){
	uint8_t nMemu=7;

	Robot.clearScreen();
	Robot.setTextSize(1);
  	textManager.writeText_P(PSTR("SERVICE"),2,7); // print "SERVICE"
	Robot.drawRect(39,18,47,11,BLACK);	//(5+1)*7-1+2+2  charWidth=5, charMargin=1, count("Enter")=6, lastItem_MarginRight=0, marginLeft==marginRight=2
	textManager.printAnyMenu(scripts_Serv_Menu,nMemu,line,col);
}


void TextManager::drawInputMenu(bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	int col=getCol(inCol);
	int lin=getLine(inLin);

	Robot.setCursor(col,lin);
	Robot.print('>');
  
	Robot.setTextColor(BLACK);
}


int TextManager::getMenuSelection(int line, int col, uint8_t nlinMenu, uint8_t pos){
	int key;
	//PS_PAD_key= -1;

	inPos=pos;
	inLin=line+pos;  //Init input cursor
	inCol=col;

	HeadMenuSelector();

	Init_PS2();
	
	while(true){
		switch(key=Robot.keyboardRead()){
			case BUTTON_UP:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvMenu(-1,nlinMenu);
				}
				break;
			case BUTTON_DOWN:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvMenu(1,nlinMenu);
				}
				break;
			case BUTTON_RIGHT:
				if(key != oldKey){
					if(F_ChoiceMusic){ // listening track
						if(inPos < nlinMenu){
							Robot.playSoundBuzz(7);
							getFaleNameFromScriptPGM(scripts_FaleNameMusic,inPos,FaleExt_wav_scr);
							Robot.playFile(fileName,IGNORE_isPlAY);
						}
					}
				}
				break;
			case BUTTON_MIDDLE:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					drawInputMenu(false); // // erase the previous text on the screen
					if(F_ChoiceMusic){
						Robot.stopPlayFile();
						Robot.playFileDisable();
					}
					oldKey=key;
					return inPos;
				}
		} // end switch(key)

		Robot.getPS2ControlInput();
		switch(PS_PAD_key){
			case BUTTON_UP:
				PS_PAD_key= -1;
				mvMenu(-1,nlinMenu);
				break;
			case BUTTON_DOWN:
				PS_PAD_key= -1;
				mvMenu(1,nlinMenu);
				break;
			case BUTTON_RIGHT:
				PS_PAD_key= -1;
				if(F_ChoiceMusic){ // listening track
					if(inPos < nlinMenu){
						getFaleNameFromScriptPGM(scripts_FaleNameMusic,inPos,FaleExt_wav_scr);
						Robot.playFile(fileName,IGNORE_isPlAY);
					}
				}
				break;
			case BUTTON_MIDDLE:
				PS_PAD_key= -1;
				drawInputMenu(false); // // erase the previous text on the screen
				if(F_ChoiceMusic){
					Robot.stopPlayFile();
					Robot.playFileDisable();
				}
				oldKey=key;
				return inPos;
		} // end switch(PS_PAD_key)

		printTimeTempC(15,12);
		if(Robot.F_enableCheckLight_1s)
			Robot.checkLight();
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		//if(F_testWiskers_1s){
		//	testAllWisker();
		//	F_testWiskers_1s=false;
		//}
		oldKey=key;
		delay(10);
	}
}



void TextManager::HeadMenuSelector(){

	oldKey= -1;
	cauntSaveScr=0;

	drawInputMenu(true); // print('>');

	updateStrTime(true);
	old_minute=minute;
	old_second=second;
	textManager.writeText(15,12,time,true);
	while (updateStrTemper());
	writeTextNch(15,2,temper,4,true);			//temperature 4bit(23.5)
	Robot.drawCircle(43,150,2,BLACK);			//temperature *
	writeTextNch(15,8,temper+6,1,true);			//temperature 1 bit (C)
	Robot.checkVoltage();
	//Serial.println(F("==1"));
}



void TextManager::HeadScreenMove(){

	Robot.fill(WHITE);
	Robot.stroke(WHITE);
	Robot.rect(0,0,128,12);						// erase the previous text
	Robot.rect(0,148,128,12);
	Robot.stroke(BLACK);

	updateStrTime(true);
	old_minute=minute;
	old_second=second;
	textManager.writeText(15,12,time,true);
	while (updateStrTemper());
	writeTextNch(15,2,temper,4,true);			//temperature 4bit(23.5)
	Robot.drawCircle(43,150,2,BLACK);			//temperature *
	writeTextNch(15,8,temper+6,1,true);			//temperature 1 bit (C)
	Robot.checkVoltage();
}


void TextManager::HeadScreenDance(){

	Robot.fill(WHITE);
	Robot.stroke(WHITE);
	Robot.rect(0,0,128, 12);					// erase the previous text
	Robot.rect(0,148,128,12);
	Robot.stroke(BLACK);

	updateStrTime(true);
#ifdef DEBUG
	Serial.println(time);
#endif

	old_minute=minute;
	old_second=second;
	textManager.writeText(15,12,time,true);
	while (updateStrTemper());
	writeTextNch(15,2,temper,4,true);			//temperature 4bit(23.5)
	Robot.drawCircle(43,150,2,BLACK);			//temperature *
	writeTextNch(15,8,temper+6,1,true);			//temperature 1 bit (C)
	Robot.checkVoltage();
}


void TextManager::HeadSaveScreen(int line, int col){

	Robot.clearScreen();
	updateStrTime(true);
	Robot.setTextSize(2);
	writeTextNch(line-1,col+6,time+6,2,true);	//second
	Robot.setTextSize(3);
	writeTextNch(line,col,time,2,true);			//hour
	writeTextNch(line,col+2,time+2,1,true);		//:
	writeTextNch(line,col+3,time+3,2,true);		//minute
	while (updateStrTemper());
	writeTextNchYX(110,9,temper,4,true);		//temperature 4bit(23.5)
	Robot.drawCircle(92,110,3,BLACK);			//temperature *
	writeTextNchYX(110,99,temper+6,1,true);		//temperature 1 bit (C)
}


void TextManager::mvMenu(int dire, uint8_t nlinMenu){

	drawInputMenu(0); // // erase the previous text on the screen

	if(dire < 0){
		if(inPos > 0){
			inPos--;
			inLin--;
		}
	}
	else{
		if(inPos < nlinMenu){
			inPos++;
			inLin++;
		}
	}
	drawInputMenu(true);
}


void TextManager::printAnyMenu(const char* const* scripts_PGM, uint8_t nlinMenu,int line, int col){

	for (int i=0; i < nlinMenu; i++){
		writeScriptPGM(scripts_PGM, i, line+i, col);
	}
}


void TextManager::printTime(int line, int col){

	textManager.writeText(line,col,time,false);
	updateStrTime(true);
	textManager.writeText(line,col,time,true);
	Serial.println(time);
}


void TextManager::printDate(int line, int col){

	textManager.writeText(line,col,date,false);
	updateStrDate(true);
	textManager.writeText(line,col,date,true);
}


void TextManager::printTimeTempC(int line, int col, bool SaveScr){

	updateTime(true);

	if(F_enableConvertC_1s){
		if(updateStrTemper()){
			printTemperC(line,col-10);
			F_enableConvertC_1s=false;
		}
	}

	if (old_hour != hour){
		fillWindowLineCol(line,1,col,2, WHITE);
		updateStrTime(false);
		writeTextNch(line,col,time,2,true);
		old_hour=hour;
	}
	if (old_minute != minute){
		if(SaveScr){
			cauntSaveScr++;
			if(cauntSaveScr == 2){
				SaveScreen(2,1);
				return;
			}
		}
		fillWindowLineCol(line,1,col+3,2, WHITE);
		updateStrTime(false);
		writeTextNch(line,col+3,time+3,2,true);
		old_minute=minute;
	}
	if (old_second != second){
		fillWindowLineCol(line,1,col+6,2, WHITE);
		updateStrTime(false);
		writeTextNch(line,col+6,time+6,2,true);
		F_enableConvertC_1s=true;
		Robot.F_enableCheckLight_1s=true;
		Robot.F_enableCheckVBat_1s=true;
		//F_testWiskers_1s=true;
		old_second=second;
	}
}


void TextManager::SaveScreen(int line, int col){

	//PS_PAD_key= -1;

	Init_PS2();

	HeadSaveScreen(line,col);

	while(true){
		if(Robot.keyboardRead() != BUTTON_NONE){
			Robot.playSoundBuzz(7);
			if(tMenu == MAIN_MENU){
				HeadMainMenu(line_s,col_s);
				HeadMenuSelector();
				return;
			}
			else if(tMenu == SERVICE_MENU){
				HeadServiceMenu(line_s,col_s);
				HeadMenuSelector();
				return;
			}
			else if(tMenu == MUSIC_MENU){
				HeadChoiceMusicMenu(line_s,col_s);
				HeadMenuSelector();
				return;
			}
			else if(tMenu == DANCE_MENU){
				HeadDanceMenu(line_s,col_s);
				HeadMenuSelector();
				return;
			}
		}
		if(PS_PAD_key != BUTTON_NONE){
			if(tMenu == MAIN_MENU){
				HeadMainMenu(line_s,col_s);
				HeadMenuSelector();
				return;
			}
			else if(tMenu == SERVICE_MENU){
				HeadServiceMenu(line_s,col_s);
				HeadMenuSelector();
				return;
			}
			else if(tMenu == MUSIC_MENU){
				HeadChoiceMusicMenu(line_s,col_s);
				HeadMenuSelector();
				return;
			}
			else if(tMenu == DANCE_MENU){
				HeadDanceMenu(line_s,col_s);
				HeadMenuSelector();
				return;
			}
		}
		Robot.getPS2ControlInput();
		printTimeTempC_Big(2,1);
		if(Robot.F_enableCheckLight_1s)
			Robot.checkLight();
		if(Robot.F_enableCheckVBat_1s)
			Robot.checkVoltage();
		//if(F_testWiskers_1s){
		//	testAllWisker();
		//	F_testWiskers_1s=false;
		//}
		delay(10); 
	}
}


void TextManager::printTimeTempC_Big(int line, int col){

	updateTime(true);

	if(F_enableConvertC_1s){
		if(updateStrTemper()){
			Robot.setTextSize(3);
			Robot.fillRect(9,110,81,30,WHITE);
			writeTextNchYX(110,9,temper,4,true); //temperature 4bit(23.5)
			F_enableConvertC_1s=false;
		}
	}

	if (old_hour != hour){
		Robot.setTextSize(3);
		fillWindowLineCol(line,1,col,2,WHITE);
		updateStrTime(false);
		writeTextNch(line,col,time,2,true);
		old_hour=hour;
	}
	if (old_minute != minute){
		Robot.setTextSize(3);
		fillWindowLineCol(line,1,col+3,2, WHITE);
		updateStrTime(false);
		writeTextNch(line,col+3,time+3,2,true);
		old_minute=minute;
	}
	if (old_second != second){
		Robot.setTextSize(2);
		fillWindowLineCol(line-1,1,col+6,2, WHITE);
		updateStrTime(false);
		writeTextNch(line-1,col+6,time+6,2,true);
		F_enableConvertC_1s=true;
		Robot.F_enableCheckLight_1s=true;
		Robot.F_enableCheckVBat_1s=true;
		//F_testWiskers_1s=true;
		old_second=second;
	}
}


void TextManager::setStrWeek(char nweek, char* dt){
	char week[4];

	switch(nweek){
		case 1:
			strcpy_P(week, PSTR("MON"));
			break;
		case 2:
			strcpy_P(week, PSTR("TUE"));
			break;
		case 3:
			strcpy_P(week, PSTR("WED"));
			break;
		case 4:
			strcpy_P(week, PSTR("THU"));
			break;
		case 5:
			strcpy_P(week, PSTR("FRI"));
			break; 
		case 6:
			strcpy_P(week, PSTR("SAT"));
			break;   
		case 7:
			strcpy_P(week, PSTR("SUN"));
			break;   
	}
	dt[0]=week[0];
	dt[1]=week[1];
	dt[2]=week[2];
}


// set string time 
void TextManager::setStrTime(char* tm, uint8_t pos){
	uint8_t b1, b2;

	switch(pos){
		case 0:
			//hour
			b2 = hour%10 + '0';
			b1 = (hour/10)%10 + '0';
			tm[0] = b1;
			tm[1] = b2;
			break;
		case 1:
			//minute
			b2 = minute%10 + '0';
			b1 = (minute/10)%10 + '0';
			tm[3] = b1;
			tm[4] = b2;
			break;
		case 2:
			//second
			b2 = second%10 + '0';
			b1 = (second/10)%10 + '0';
			tm[6] = b1;
			tm[7] = b2;
			break;
	}
}

// set string date 
void TextManager::setStrDate(char* dt, uint8_t pos){
	uint8_t b1, b2;

	switch(pos){
		case 3:
			//week
			setStrWeek(dayOfWeek, date);
			break;
		case 4:
			//dayOfMonth
			b2 = dayOfMonth%10 + '0';
			b1 = (dayOfMonth/10)%10 + '0';
			dt[4] = b1;
			dt[5] = b2;
			break;
		case 5:
			//month
			b2 = month%10 + '0';
			b1 = (month/10)%10 + '0';
			dt[7] = b1;
			dt[8] = b2;
			break;
		case 6:
			//year
			b2 = year%10 + '0';
			b1 = (year/10)%10 + '0';
			dt[12] = b1;
			dt[13] = b2;
			break;
	}
}


void TextManager::updateStrTime(bool refresh){
	uint8_t b1, b2;

	RTC.get(rtc,refresh);

	second = rtc[0];
	minute = rtc[1];
	hour = rtc[2];

	for(uint8_t i=0; i < 3; i++){
		setStrTime(time,i);
	}
}


void TextManager::updateTime(bool refresh){

	RTC.get(rtc,refresh);

	second = rtc[0];
	minute = rtc[1];
	hour = rtc[2];
}


void TextManager::updateStrDate(bool refresh){
	uint8_t b1, b2;

	RTC.get(rtc,refresh);

	dayOfWeek = rtc[3];
	dayOfMonth = rtc[4];
	month = rtc[5];
	year = rtc[6];

	for(uint8_t i=3; i < 7; i++){
		setStrDate(date,i);
	}
}


//line=4, col=7 - time
//line=6, col=4 - date
void TextManager::getTimeSelection(int line, int col){
	int key;

	inPos=7;
	inLin=line;  //Init input cursor
	inCol=col;
	inLin_s=line;
	inCol_s=col;

	initFlagsForAnyState();

  	textManager.writeText_P(PSTR("SET TIME"),1,7);
	Robot.drawRect(39,8,53,11,BLACK);	//(5+1)*7-1+2+2  charWidth=5, charMargin=1, count("Enter")=6, lastItem_MarginRight=0, marginLeft==marginRight=2
	updateStrTime(true);
	textManager.writeText(inLin,inCol,time);
	updateStrDate(false);
	textManager.writeText(inLin_s+2,inCol_s-3,date);
	textManager.writeText_P(PSTR("EXIT"),inLin_s+4,16);
	if(inPos == 7)
		Robot.drawRect(93,78,29,11,BLACK); //print rect.
	else
		drawInput(inLin,inCol,2,true); // print('_');

	Init_PS2();

	while(true){
		switch(key=Robot.keyboardRead()){
			case BUTTON_UP:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvTimePadSelect(BUTTON_UP);
				}
				break;
			case BUTTON_DOWN:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvTimePadSelect(BUTTON_DOWN);
				}
				break;
			case BUTTON_LEFT:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvTimePadSelect(BUTTON_LEFT);
				}
				break;
			case BUTTON_RIGHT:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvTimePadSelect(BUTTON_RIGHT);
				}
				break;
			case BUTTON_MIDDLE:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					if(inPos != 7)
						setTime();
					//Robot.clearScreen();
					return;
				}
		}
		Robot.getPS2ControlInput();
		if(inPos != 7)
			runKnobTime(inPos);
		delay(10);
		oldKey=key;
	}
}


void TextManager::mvTimePadSelect(int select){
	uint8_t wSel;

	switch(select){
		case BUTTON_UP:
			if((inPos >= 3) && (inPos < 7)){
				drawInput(inLin,inCol,4,false); // clear ('_');
				inPos=0;
				wSel=getTimeColLine(inPos);
				drawInput(inLin,inCol,wSel,true); // print ('_');
			}
			else if(inPos == 7){
				Robot.drawRect(93,78,29,11,WHITE); //clear rect.
				inPos=0;
				wSel=getTimeColLine(inPos);
				drawInput(inLin,inCol,wSel,true); // print ('_');
			}
			break;
		case BUTTON_DOWN:
			if(inPos < 3){
				drawInput(inLin,inCol,4,false); // clear ('_');
				inPos=3;
				wSel=getTimeColLine(inPos);
				drawInput(inLin,inCol,wSel,true); // print ('_');
			}
			if((inPos >= 3) && (inPos < 7)){
				drawInput(inLin,inCol,4,false); // clear ('_');
				inPos=7;
				getTimeColLine(inPos);
				Robot.drawRect(93,78,29,11,BLACK); //print rect.
			}
			break;
		case BUTTON_LEFT:
			if(inPos > 0){
				if(inPos == 7)
					Robot.drawRect(93,78,29,11,WHITE); //clear rect.
				else
					drawInput(inLin,inCol,4,false); // clear ('_');
				inPos--;
				wSel=getTimeColLine(inPos);
				drawInput(inLin,inCol,wSel,true); // print ('_');
			}
			break;
		case BUTTON_RIGHT:
			if(inPos <= 6){
				drawInput(inLin,inCol,4,false); // clear ('_');
				inPos++;
				wSel=getTimeColLine(inPos);
				if(inPos == 7)
					Robot.drawRect(93,78,29,11,BLACK); //print rect.
				else
					drawInput(inLin,inCol,wSel,true); // print ('_');
			}
			break;
	}
}


uint8_t TextManager::getTimeColLine(int pos){
	uint8_t wSel;

	switch(pos){
		case 0:
			inLin=inLin_s;
			inCol=inCol_s;
			wSel=2;
			break;
		case 1:
			inLin=inLin_s;
			inCol=inCol_s+3;
			wSel=2;
			break;
		case 2:
			inLin=inLin_s;
			inCol=inCol_s+6;
			wSel=2;
			break;
		case 3:
			inLin=inLin_s+2;
			inCol=inCol_s-3;
			wSel=3;
			break;
		case 4:
			inLin=inLin_s+2;
			inCol=inCol_s-3+4;
			wSel=2;
			break;
		case 5:
			inLin=inLin_s+2;
			inCol=inCol_s-3+7;
			wSel=2;
			break;
		case 6:
			inLin=inLin_s+2;
			inCol=inCol_s-3+10;
			wSel=4;
			break;
	}
	return wSel;
}



void TextManager::runKnobTime(uint8_t pos){
	int8_t val;

	if((val=knobGetDigitTime(pos)) >= 0){

		switch(pos){
			case 0:
				if(val < 24){
					hour=val;
					writeTextNch(inLin_s,inCol_s,time,2,false);
					setStrTime(time,0);
					writeTextNch(inLin_s,inCol_s,time,2,true);
				}
				break;
			case 1:
				if(val < 60){
					minute=val;
					writeTextNch(inLin_s,inCol_s+3,time+3,2,false);
					setStrTime(time,1);
					writeTextNch(inLin_s,inCol_s+3,time+3,2,true);
				}
				break;
			case 2:
				if(val < 60){
					second=val;
					writeTextNch(inLin_s,inCol_s+6,time+6,2,false);
					setStrTime(time,2);
					writeTextNch(inLin_s,inCol_s+6,time+6,2,true);
				}
				break;
			case 3:
				if(val < 8){
					dayOfWeek=val;
					writeTextNch(inLin_s+2,inCol_s-3,date,3,false);
					setStrDate(date,3);
					writeTextNch(inLin_s+2,inCol_s-3,date,3,true);
				}
				break;
			case 4:
				if(val < 32){
					dayOfMonth=val;
					writeTextNch(inLin_s+2,inCol_s-3+4,date+4,2,false);
					setStrDate(date,4);
					writeTextNch(inLin_s+2,inCol_s-3+4,date+4,2,true);
				}
				break;
			case 5:
				if(val < 13){
					month=val;
					writeTextNch(inLin_s+2,inCol_s-3+7,date+7,2,false);
					setStrDate(date,5);
					writeTextNch(inLin_s+2,inCol_s-3+7,date+7,2,true);
				}
				break;
			case 6:
				if(val < 60){
					year=val;
					writeTextNch(inLin_s+2,inCol_s-3+12,date+12,2,false);
					setStrDate(date,6);
					writeTextNch(inLin_s+2,inCol_s-3+12,date+12,2,true);
				}
				break;
		}
	}

}


int8_t TextManager::knobGetDigitTime(uint8_t pos){
	uint8_t val;

	switch(pos){
		case 0://hour
			val=map(Robot.knobRead(),0,1023,0,25);
			if(val > 23)
				val=23;
			break;
		case 1://minute
		case 2://second
		case 6://year
			val=map(Robot.knobRead(),0,1023,0,62);
			if(val > 59)
				val=59;
			break;
		case 3://dayOfWeek
			val=map(Robot.knobRead(),0,1023,1,9);
			if(val > 7)
				val=7;
			break;
		case 4://dayOfMonth
			val=map(Robot.knobRead(),0,1023,0,33);
			if(val > 31)
				val=31;
			break;
		case 5://month
			val=map(Robot.knobRead(),0,1023,1,14);
			if(val > 12)
				val=12;
			break;
			
	}
	if(val==oldKnobVal){
		return -1;	//No changes
	}
	else{
		oldKnobVal=val;
		return val;
	}
}

void TextManager::setTime(){

	RTC.stop();
	RTC.set(DS1307_SEC, second);		//set the seconds
	RTC.set(DS1307_MIN, minute);		//set the minutes
	RTC.set(DS1307_HR, hour);			//set the hours
	RTC.set(DS1307_DOW, dayOfWeek);		//set the day of the week
	RTC.set(DS1307_DATE, dayOfMonth);	//set the date
	RTC.set(DS1307_MTH, month);			//set the month
	RTC.set(DS1307_YR, year);			//set the year
	RTC.start();
}



bool TextManager::updateStrTemper(){
	int tC;
	byte dataC[2];
	char stringC[8];

		if(!F_convertDelay){ // start
			DS_18B20.reset();
			DS_18B20.write(0xCC);
			DS_18B20.write(0x44);
			F_convertDelay=true;
			startConvert=millis();
		}
		else if(F_convertDelay){ // end
			if((millis() - startConvert < TIME_CONVERTION_9BIT)){
				return false;
			}
			else{ // >= TIME_CONVERTION_9BIT (95ms)
				F_convertDelay=false;
				DS_18B20.reset();
				DS_18B20.write(0xCC); // Skip ROM command
				DS_18B20.write(0xBE); // Read EEPROM
				dataC[0] = DS_18B20.read();
				dataC[1] = DS_18B20.read();
				DS_18B20.reset();
				tC = (dataC[1]<< 8)+dataC[0];
				tC = tC & 0xFFF8;
				if(tC != tC_old){
					tC_old=tC;
				#ifdef DEBUG
					if (F_DebugOutput) {
						if (F_DebugTemperatC) {
							Serial.println(F("tC:"));
							Serial.print(dataC[1],HEX);
							Serial.print(dataC[0],HEX);
							Serial.print(F(" "));
							Serial.print(tC,HEX);
							Serial.print(F(" "));
						}
					}  
				#endif
					tC = tC>>4;
					itoa(tC,stringC,10); //convert int to string base 10
					strncpy (temper,stringC,2); // copy n bytes to string
					if((dataC[0] & 0x08) != 0){
						temper[3]='5';
					}
					else{
						temper[3]='0';
					}
			#ifdef DEBUG
				if (F_DebugOutput) {
					if (F_DebugTemperatC) {
						Serial.print(tC,HEX);
						Serial.print(F(" "));
						Serial.println(temper);
					}
				}  
			#endif
				return true;
				}
			}
		}
		return false;
}



void TextManager::setEnableConvertion(bool flag){

	F_enableConvertC_1s=flag;
}


void TextManager::printTemperC(int line, int col){

	fillWindowLineCol(line,1,col,4, WHITE);
	textManager.writeTextNch(line,col,temper,4,true);
}


void TextManager::setResolution_9bit(){

	DS_18B20.reset();
	DS_18B20.write(0xCC);
	DS_18B20.write(0x4E); // Write to EEPROM 3 bytes
	DS_18B20.write(55); // high alarm temp
	DS_18B20.write(55); // low alarm temp
	DS_18B20.write(0x1F); // configuration register, 9bit resolution
	DS_18B20.reset();
}


void TextManager::readScratchPadDS18B20(){
	uint8_t ScratchPad[9];

	DS_18B20.reset();
	DS_18B20.write(0xCC);
	DS_18B20.write(0xBE); // Read EEPROM
	for(uint8_t i=0; i < 9; i++){
		ScratchPad[i] = DS_18B20.read();
	}
	DS_18B20.reset();

#ifdef DEBUG
	Serial.print(F("DS18B20= "));
	for (uint8_t i = 0; i < 9; i++){
		if (ScratchPad[i] < 16) Serial.print(F("0"));
			Serial.print(ScratchPad[i], HEX);
		Serial.print(F(" "));
	}
	Serial.println();
#endif
}


//line=4, col=13 - Max. Picture Collection
void TextManager::getMaxPicCollSelect(int line, int col){
	int key;
	int8_t val;

	inPos=1;
	inLin=line;  //Init input cursor
	inCol=col;
	inLin_s=line;
	inCol_s=col;

	//initFlagsForAnyState();

	val=Robot.readByte(ADDR_MAX_PICT_COLL);
#ifdef DEBUG
	Serial.print(F("SMaxPicColl: "));
	Serial.println(val, DEC);
#endif

  	textManager.writeText_P(PSTR("Set Max. files"),2,1);
  	textManager.writeText_P(PSTR("in Picture"),3,1);
  	textManager.writeText_P(PSTR("Collection:"),4,1);
	textManager.writeText_P(PSTR("[  ]"),4,16);
	textManager.writeText(4,17,val);


	textManager.writeText_P(PSTR("EXIT"),inLin_s+2,13);
	if(inPos == 1)
		Robot.drawRect(75,58,29,11,BLACK); //print rect.
	else
		drawInput(inLin,inCol,2,true); // print('_');

	Init_PS2();

	while(true){
		switch(key=Robot.keyboardRead()){
			case BUTTON_UP:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvMaxPicPadSelect(BUTTON_UP);
				}
				break;
			case BUTTON_DOWN:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvMaxPicPadSelect(BUTTON_DOWN);
				}
				break;
			case BUTTON_LEFT:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvMaxPicPadSelect(BUTTON_LEFT);
				}
				break;
			case BUTTON_RIGHT:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					mvMaxPicPadSelect(BUTTON_RIGHT);
				}
				break;
			case BUTTON_MIDDLE:
				if(key != oldKey){
					Robot.playSoundBuzz(7);
					if(inPos == 0)
						setMaxPicColl(val);
					//Robot.clearScreen();
					return;
				}
		}
		Robot.getPS2ControlInput();
		if(inPos != 1)
			val=runKnobMaxPic();
		delay(10);
		oldKey=key;
	}
}


void TextManager::mvMaxPicPadSelect(int select){

	switch(select){
		case BUTTON_UP:
			if(inPos == 0){
				inPos=1;
				drawInput(inLin,inCol,2,false); // clear ('_');
				Robot.drawRect(75,58,29,11,BLACK); //print rect.
				break;
			}
			else if(inPos == 1){
				inPos=0;
				drawInput(inLin,inCol,2,true); // print ('_');
				Robot.drawRect(75,58,29,11,WHITE); //clear rect.
			}
			break;
		case BUTTON_DOWN:
			if(inPos == 0){
				inPos=1;
				drawInput(inLin,inCol,2,false); // clear ('_');
				Robot.drawRect(75,58,29,11,BLACK); //print rect.
				break;
			}
			else if(inPos == 1){
				inPos=0;
				drawInput(inLin,inCol,2,true); // print ('_');
				Robot.drawRect(75,58,29,11,WHITE); //clear rect.
			}
			break;
		case BUTTON_LEFT:
			if(inPos == 0){
				inPos=1;
				drawInput(inLin,inCol,2,false); // clear ('_');
				Robot.drawRect(75,58,29,11,BLACK); //print rect.
				break;
			}
			else if(inPos == 1){
				inPos=0;
				drawInput(inLin,inCol,2,true); // print ('_');
				Robot.drawRect(75,58,29,11,WHITE); //clear rect.
			}
			break;
		case BUTTON_RIGHT:
			if(inPos == 0){
				inPos=1;
				drawInput(inLin,inCol,2,false); // clear ('_');
				Robot.drawRect(75,58,29,11,BLACK); //print rect.
				break;
			}
			else if(inPos == 1){
				inPos=0;
				drawInput(inLin,inCol,2,true); // print ('_');
				Robot.drawRect(75,58,29,11,WHITE); //clear rect.
			}
			break;
	} // end switch(select)
}


uint8_t TextManager::runKnobMaxPic(){
	int8_t val;

	if((val=knobGetDigitMaxPic()) >= 0){
		if(val >= MAX_PIC_COLL)
			val=MAX_PIC_COLL-1;
		debugPrint(val,inLin_s,inCol_s);
		return val;
	}
	return oldKnobVal;
}


int8_t TextManager::knobGetDigitMaxPic(){
	uint8_t val;

	val=map(Robot.knobRead(),0,1023,2,100);

	if(val == oldKnobVal){
		return -1;	//No changes
	}
	else{
		oldKnobVal=val;
		return val;
	}
}


void TextManager::setMaxPicColl(uint8_t val){

	Robot.writeByte(ADDR_MAX_PICT_COLL,val);
#ifdef DEBUG
	Serial.print(F("SMaxPicColl: "));
	Serial.print(val, DEC);
	delay(100);
	val=Robot.readByte(ADDR_MAX_PICT_COLL);
	Serial.print(F("   "));
	Serial.print(F("RMaxPicColl: "));
	Serial.println(val, DEC);
#endif
}

void testAllWisker(){

	Robot.getWiskerLF();
	Robot.getWiskerRF();
	Robot.getWiskerLR();
	Robot.getWiskerRR();
}



TextManager textManager=TextManager(); 
