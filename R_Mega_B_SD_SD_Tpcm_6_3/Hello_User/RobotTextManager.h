#ifndef ROBOTTEXTMANAGER_H
#define ROBOTTEXTMANAGER_H

//************************************************************************************
#define USERNAME		0
#define ROBOTNAME		1
#define CITYNAME		2
#define COUNTRYNAME		3
#define EMPTY			4
#define MAIN_MENU		0
#define SERVICE_MENU	1
#define MUSIC_MENU		2
#define DANCE_MENU		3

#define TIME_CONVERTION_9BIT	95
#define BUFFPIXEL		20
//************************************************************************************



class TextManager{
	public:
		TextManager();
		//add some margin to the text, left side only atm.
		void setMargin(int margin_left,int margin_top);

		//print text based on line, column. 
		void writeText(int line, int col, const char* str, bool onOff=true);
		void writeText(int line, int col, char val, bool onOff=true);
		void writeText(int line, int col, int val, bool onOff=true);
		void writeText(int line, int col, long val, bool onOff=true);
		void writeText(int line, int col, float valFl, bool onOff=true);
		void writeTextYX(int y, int x, float valFl, bool onOff=true);
		void writeTextNch(int line, int col, char* txt, uint8_t nchar, bool onOffonOff=true);
		void writeTextNchYX(int y, int x, char* txt, uint8_t nchar, bool onOff);
		void getPGMtextToBuffer(const char* const* const scripts_P,int seq);

		//debug print
		void debugPrint(const char* value, int line, int col);
		void debugPrint(char value, int line, int col);
		void debugPrint(int value, int line, int col);
		void debugPrint(long value, int line, int col);
		void debugPrint(float value, int line, int col);
		void debugPrintYX(float valueFl, int y, int x);
		//print a script from the scripts library
		void writeScriptPGM(const char* const* scripts_PGM,int seq, int line, int col);
		void writeText_P(const char* scripts_P, int line, int col, bool onOff=true);
		void fillWindowLineCol(int line, int nline, int col, int ncol, uint16_t c=WHITE);

		//The whole process of getting input
		void inputUser(int line,int col, int code);
		uint8_t inputMainMenu(int line,int col, uint8_t pos);
		uint8_t inputServiceMenu(int line,int col, uint8_t pos);
		uint8_t inputChoiceMusicMenu(int line,int col, uint8_t pos);
		uint8_t inputDanceMenu(int line,int col, uint8_t pos);
		//Print a cursor and virtual keyboard on screen, and save the user's input
		void getInputUser(int line, int col);  
		//Get user name, robot name, city name or country name from EEPROM
		//and store in the input pool.
		void setInputPoolUser(int code);
		//save user input to EEPROM
		void pushInputUser(int code);

		int getMenuSelection(int lin, int col, uint8_t nlinMenu, uint8_t pos);  
		void printAnyMenu(const char* const* const scripts_PGM, uint8_t nlinMenu, int line, int col);
		void printTime(int line, int col);
		void printTimeTempC(int line, int col, bool SaveScr=true);
		void printTimeTempC_Big(int line, int col);
		void printDate(int line, int col);
		void getTimeSelection(int line, int col);
		void getMaxPicCollSelect(int line, int col);
		int8_t knobGetDigitTime(uint8_t pos);
		void runKnobTime(uint8_t pos);
		int8_t knobGetDigitMaxPic();
		uint8_t runKnobMaxPic();
		void setTime();
		void setMaxPicColl(uint8_t val);
		void updateTime(bool refresh);
		void SaveScreen(int line, int col);
		void HeadScreenMove();
		void HeadScreenDance();

		uint8_t rtc[7];
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
		uint8_t dayOfWeek;
		uint8_t dayOfMonth;
		uint8_t month;
		uint8_t year;
		long oldValue;
		float old_valueFl;
		uint8_t old_second;
		uint8_t old_minute;
		uint8_t old_hour;
		uint8_t cauntSaveScr;
		int line_s;
		int col_s;

		//DS18B20 Temperature sensor
		uint8_t DSdata[2];
		int temp;
		static bool F_convertDelay;
		uint32_t startConvert; 
		bool updateStrTemper();
		void printTemperC(int line, int col);
		void setEnableConvertion(bool flag);
		void setResolution_9bit();
		void readScratchPadDS18B20();
		int tC_old;
		static bool F_enableConvertC_1s;
		char temper[8];
		char PGMbuffer[85];  //the buffer for storing strings

	private:
		int margin_left,margin_top;
		int getLine(int line);	//Convert line to pixels from top
		int getCol(int col);	//Convert line to pixels from left

		static const int lineHeight;//8+2=10
		static const int charWidth; //5+1=6

		uint8_t inPos;
		int inLin, inLin_s;
		int inCol, inCol_s;

		void drawInput(int line, int col, uint8_t wSel, bool onOff);
		void drawInputMenu(bool onOff);
		void mvUser(int dire);
		void mvMenu(int dire, uint8_t pos);
		void mvTimePadSelect(int select);
		void mvMaxPicPadSelect(int select);

		char selectLetter();
		void refreshCurrentLetter(char letter);

		//void getPGMtextMenu(int seq);
		void setStrWeek(char nweek, char* dt);
		void updateStrTime(bool refresh);
		void updateStrDate(bool refresh);
		void setStrTime(char* tm, uint8_t pos);
		void setStrDate(char* dt, uint8_t pos);
		uint8_t getTimeColLine(int pos);
		void HeadMainMenu(int line, int col);
		void HeadDanceMenu(int line, int col);
		void HeadChoiceMusicMenu(int line, int col);
		void HeadServiceMenu(int line, int col);
		void HeadMenuSelector();
		void HeadSaveScreen(int line, int col);

		char inputPool[18];
		char time[9];
		char date[15];
		uint8_t oldKnobVal;
		int8_t tMenu;
};

extern TextManager textManager;


#endif
