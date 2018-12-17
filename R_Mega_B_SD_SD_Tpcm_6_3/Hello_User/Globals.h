
/*
//default tempo and pitch of the music
int tempo = 60;
int pitch = 1000;
byte volume = 4;
static int oldKey;


char strParm1[13];
char strParm2[8];
char strParm3[8];
uint32_t TimerDispCompas, TimerDispProsses;
bool F_compas, F_StartDispCompas, F_DisplayProsses, F_DrawBMP;

uint16_t		UDistance[8]={0,0,0,0,0,0,0,0};
uint32_t		UDistanceSum = 0;
uint8_t		iUDistance = 0;

--------------------------------------------------------------------

void TextManager::writeText(int line, int col, char* txt, bool onOff){

	if(!onOff)
		Robot.setTextColor(WHITE);

	Robot.setCursor(getCol(col),getLine(line));
	Robot.print(txt);
  
	Robot.setTextColor(BLACK);
}

void RobotControl::debugPrint(long value, uint8_t x, uint8_t y){
	static long oldVal=0;

	Arduino_LCD::stroke(backGround);
	text(oldVal,x,y);
	Arduino_LCD::stroke(foreGround);
	text(value,x,y);
	oldVal=value;
}

void Adafruit_GFX::text(int value, uint8_t x, uint8_t y){
  if (!useStroke)
    return;
  
  setTextWrap(false);
  setTextColor(strokeColor);
  setCursor(x, y);
  print(value);
}
void Adafruit_GFX::text(long value, uint8_t x, uint8_t y){
  if (!useStroke)
    return;
  
  setTextWrap(false);
  setTextColor(strokeColor);
  setCursor(x, y);
  print(value);
}
void Adafruit_GFX::text(char value, uint8_t x, uint8_t y){
  if (!useStroke)
    return;
  
  setTextWrap(false);
  setTextColor(strokeColor);
  setCursor(x, y);
  print(value);
}

void Adafruit_GFX::text(const char * text, int16_t x, int16_t y) {
  if (!useStroke)
    return;
  
  setTextWrap(false);
  setTextColor(strokeColor);
  setCursor(x, y);
  print(text);
}


  // start
  Robot.fill(255, 255, 255);
  Robot.stroke(255, 255, 255);
  Robot.rect(0, 0, 128, 80); // erase the previous text
  Robot.stroke(0, 0, 0);
  Robot.text("Start", 5, 5);



void DoSetTime(char* str){
	uint8_t setTime;

	ParseStr1Par(str);
	setTime=(uint8_t)atoi(strParm1);


	Serial.print(F("setTime("));
	PrintStr(strParm1, strlen(strParm1));
	Serial.println(F(")"));

	if(setTime){
		RTC.stop();
		RTC.set(DS1307_SEC, 0);        //set the seconds
		RTC.set(DS1307_MIN, 34);       //set the minutes
		RTC.set(DS1307_HR, 13);        //set the hours
		RTC.set(DS1307_DOW, 6);        //set the day of the week
		RTC.set(DS1307_DATE, 29);      //set the date
		RTC.set(DS1307_MTH, 11);       //set the month
		RTC.set(DS1307_YR, 14);        //set the year
		RTC.start();
	}

	int year = RTC.get(DS1307_YR,true);
	int month = RTC.get(DS1307_MTH,false);
	int dayOfMonth = RTC.get(DS1307_DATE,false);
	int dayOfWeek = RTC.get(DS1307_DOW,false);
	int hour = RTC.get(DS1307_HR,false);
	int minute = RTC.get(DS1307_MIN,false);
	int second = RTC.get(DS1307_SEC,false);


	Serial.println("Time:");
	Serial.print("YR= "); Serial.print(year ,DEC);Serial.println(";");
	Serial.print("MTH= "); Serial.print(month ,DEC);Serial.println(";");
	Serial.print("DATE= "); Serial.print(dayOfMonth ,DEC);Serial.println(";");
	Serial.print("TR= "); Serial.print(dayOfWeek ,DEC);Serial.println(";");
	Serial.print("HR= "); Serial.print(hour ,DEC);Serial.println(";");
	Serial.print("MIN= "); Serial.print(minute ,DEC);Serial.println(";");
	Serial.print("SEC= "); Serial.print(second ,DEC);Serial.println(";");


}


*/


