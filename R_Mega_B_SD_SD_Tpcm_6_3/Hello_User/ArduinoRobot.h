#ifndef ArduinoRobot_h
#define ArduinoRobot_h

#include "Arduino.h"
#include <PS2X_lib.h>
#include <Ultrasonic.h>
#include "OneWire.h"
#include "Arduino_LCD.h"
#include "EasyTransfer2.h"
#include "EEPROM_I2C.h"
#include "Compass.h"
#include "SD.h"


//************************************************************************************
#define DC_LCD			46
#define RST_LCD			47
#define LCD_CS			48

#define ECHO_FR			23
#define TRIG_FR			25
#define ECHO_RV			22
#define TRIG_RV			24

#define DATA_DS18B20	12
#define LIGHT_ON		11
#define TMP_REDY		10 // TMPPCM_REDY pin to input (Hight if playing) 

#define WISKER_LF		A0
#define WISKER_LR		A1
#define WISKER_RR		A2
#define WISKER_RF		A3
#define V_LIGHT			A4
//bottom bord
#define ORS_ON			B_TKD4 //D12 
#define NEOP			B_TKD3 //D4




#define BUTTON_NONE	   -1
#define BUTTON_LEFT		0
#define BUTTON_DOWN		1
#define BUTTON_UP		2
#define BUTTON_RIGHT    3
#define BUTTON_MIDDLE   4
#define NUMBER_BUTTONS	5

#define IGNORE_isPlAY	0
#define SKEEP_isPlAY	1

// image locations on the EEPROM
#define HOME_BMP		0
#define BATTERY_BMP		2048
#define COMPASS_BMP		4096
#define CONTROL_BMP		6144
#define GEARS_BMP		8192
#define LIGHT_BMP		10240
#define OSCILLO_BMP		12288
#define VOLT_BMP		14336
#define INICIO_BMP		16384  // this is a full screen splash

//Command code
#define COMMAND_SWITCH_MODE			0
#define COMMAND_RUN					10
#define COMMAND_MOTORS_STOP			11
#define COMMAND_LEDS_SHOW			15
#define COMMAND_LEDS_IS_SHOW		16
#define COMMAND_LEDS_IS_SHOW_RE		17
#define COMMAND_LEDS_STOP			18
#define COMMAND_PAUSE_SHOW_LEDS		19
#define COMMAND_ANALOG_WRITE		20
#define COMMAND_DIGITAL_WRITE		30
#define COMMAND_ANALOG_READ			40
#define COMMAND_ANALOG_READ_RE		41
#define COMMAND_DIGITAL_READ		50
#define COMMAND_DIGITAL_READ_RE		51
#define COMMAND_READ_IR				60
#define COMMAND_READ_IR_RE			61
#define COMMAND_ACTION_DONE			70
#define COMMAND_READ_TRIM			80
#define COMMAND_READ_TRIM_RE		81
#define COMMAND_READ_POT			82
#define COMMAND_READ_POT_RE			83
#define COMMAND_ANALOG_READ_KEY		84
#define COMMAND_ANALOG_READ_KEY_RE	85
#define COMMAND_AVERAGE_READ_KEY	86
#define COMMAND_AVERAGE_READ_KEY_RE	87
#define COMMAND_CALIBRATE_KEY		88 
#define COMMAND_PAUSE_MODE			90
#define COMMAND_LINE_FOLLOW_CONFIG	100
#define COMMAND_PLAY_FILE			110
#define COMMAND_PLAY_FILE_TIME		111
#define COMMAND_STOP_PLAY_FILE		112
#define COMMAND_PLAY_DISABLE		113
#define COMMAND_CHANGE_VOLUME		114
#define COMMAND_SET_VOLUME			115
#define COMMAND_PAUSE_PLAY			116
#define COMMAND_LOOP_PLAY			117
#define COMMAND_IS_PLAYING			118
#define COMMAND_IS_PLAYING_RE		119
#define COMMAND_QUALITY_PLAYING		120
#define COMMAND_PLAY_BUZZ			125
#define COMMAND_PLAY_BUZZ_RE		126


//component codename
#define CN_LEFT_MOTOR		0
#define CN_RIGHT_MOTOR		1
#define CN_IR				2

//motor board modes
#define MODE_SIMPLE			0
#define MODE_LINE_FOLLOW	1
#define MODE_ADJUST_MOTOR	2
#define MODE_IR_CONTROL		3

//port types, for R/W
#define TYPE_TOP_TK			0
#define TYPE_TOP_TKD		1
#define TYPE_BOTTOM_TKD		2
#define TYPE_BOTTOM_TK		3
#define TYPE_BOTTOM_SENSE	4

//top TKs
#define T_TK0		100
#define T_TK1		101
#define T_TK2		102
#define T_TK3		103
#define T_TK4		104
#define T_TK5		105
#define T_TK6		106
#define T_TK7		107
#define T_BCHRG		108
#define T_BON		109

//top TKDs, just for communication purpose
#define T_TKD0		116
#define T_TKD1		117
#define T_TKD2		118
#define T_TKD3		119
#define T_TKD4		120
#define T_TKD5		121
#define T_LED1		122

//bottom TKDs, just for communication purpose
#define B_TKD1		201
#define B_TKD2		202
#define B_TKD3		203 // NEOP, D4
#define B_TKD4		204	// ORS_ON, D12
//Sense
#define B_SENSE_A	205 // CSA 
#define B_SENSE_B	206 // CSB

//bottom IRs, Multiplexer for communication purpose
#define B_IR0		210
#define B_IR1		211
#define B_IR2		212
#define B_IR3		213
#define B_IR4		214
#define B_VBAT		215 // R1=10k, R2=5,6k

//320 - 337 username, 
#define ADDRESS_USERNAME	320
//338 - 355 robotname, 
#define ADDRESS_ROBOTNAME	338
//356 - 373 cityname, 
#define ADDRESS_CITYNAME	356
 //374- 391 countryname, 
#define ADDRESS_COUNTRYNAME 374
//392-409 robot info
#define ADDRESS_ROBOTINFO	392
#define ADDR_CHOICE_MUSIC	410
#define ADDR_MAX_PICT_COLL	411

// for TFT LCD
#define	BLACK	ILI9163C_BLACK   
#define	BLUE	ILI9163C_BLUE    
#define	RED		ILI9163C_RED     
#define	GREEN	ILI9163C_GREEN   
#define CYAN	ILI9163C_CYAN    
#define MAGENTA	ILI9163C_MAGENTA 
#define YELLOW	ILI9163C_YELLOW    
#define WHITE	ILI9163C_WHITE   

// for leds_show
#define RAINBOW						1

#define CYLON_UP					2
#define CYLON_DOUN					3
#define CYLON_UP_DOUN				4

#define CASCADE_UP					5
#define CASCADE_DOUN				6
#define CASCADE_UP_DOUN				7

#define COLOR_WIPE_W_UP				8
#define COLOR_WIPE_W_DOUN			9
#define COLOR_WIPE_W_UP_DOUN		10

#define COLOR_WIPE_R_UP				11
#define COLOR_WIPE_R_DOUN			12
#define COLOR_WIPE_R_UP_DOUN		13

#define COLOR_WIPE_G_UP				14
#define COLOR_WIPE_G_DOUN			15
#define COLOR_WIPE_G_UP_DOUN		16

#define COLOR_WIPE_B_UP				17
#define COLOR_WIPE_B_DOUN			18
#define COLOR_WIPE_B_UP_DOUN		19

#define COLOR_WIPE_RGBW_UP			20
#define COLOR_WIPE_RGBW_DOUN		21
#define COLOR_WIPE_RGBW_UP_DOUN		22

#define THEATRE_CHASE_W_UP			23
#define THEATRE_CHASE_W_DOUN		24
#define THEATRE_CHASE_W_UP_DOUN		25

#define THEATRE_CHASE_R_UP			26
#define THEATRE_CHASE_R_DOUN		27
#define THEATRE_CHASE_R_UP_DOUN		28

#define THEATRE_CHASE_G_UP			29
#define THEATRE_CHASE_G_DOUN		30
#define THEATRE_CHASE_G_UP_DOUN		31

#define THEATRE_CHASE_B_UP			32
#define THEATRE_CHASE_B_DOUN		33
#define THEATRE_CHASE_B_UP_DOUN		34

#define THEATRE_CHASE_RGBW_UP		35
#define THEATRE_CHASE_RGBW_DOUN		36
#define THEATRE_CHASE_RGBW_UP_DOUN	37

#define RAINBOW_1					38
#define RAINBOW_CYCLE				39

#define THEATRE_CHASE_RAINBOW_UP		40
#define THEATRE_CHASE_RAINBOW_DOUN		41
#define THEATRE_CHASE_RAINBOW_UP_DOUN	42


#define SHOW_CONTINUE				255

#define SHOW_ALL_STYLES_UP			255
#define SHOW_ALL_STYLES_DOUN		254
#define SHOW_ALL_STYLES_RANDOM		253



#define RUN					0
#define DISCO_BOT			1
#define PICTURE_BROWSER		2
#define LINE_FOLLOW			3
#define RESCUE_BOT			4
#define RUN_PATTERN			5
#define SERVICE				6

#define MACARENA			0
#define LAMBADA				1
#define KISS_KISS			2
#define AMERICANA			3
#define EMPTY				4
#define EMPTY1				5

#define DANCE_MENU_LINE		7
#define MAIN_MENU_LINE		7
#define SERVICE_MENU_LINE	7

#define MAXNUM_MUSIC		6

#define SET_USER_DATA		0
#define GET_INFO			1
#define SET_TIME			2
#define WHEEL_CALIBRATION	3
#define CHOICE_MUSIC		4
#define SET_MAX_PIC_COLL	5

#define INIT_FOR_MOVE		0
#define ERROR_STATE			1
#define POWER_STATE_OFF		2
#define TURN_TO_OPENING		3
#define RUN_DANCE			3
#define STOP_STATE			4
#define RETURN_STATE		5
#define ANY_STATE			255


#define	DBGSerial			Serial
#ifdef  DBGSerial
  #define DEBUG
#endif
#define DEBUG_BAUD			9600	// 9600, Serial DEBUG_BAUD rate
#define OPT_TERMINAL_MONITOR
#define DEBUG_ULTRASONIC
#define NUM_EEPROM_BMP		10

#define MAX_PIC_COLL		100

#define TURN_OFF_VBAT		380		// 3,8V
#define MAX_VBAT			1300	// 13V
#define MAX_COUNT_VBAT		10		// MAX counter failure voltage 

#define MAX_CHECK_WISKERS	2		// MAX check wiskers

//************************************************************************************


/*
A message structure for Top Bord will be:
analogWrite (3):
	byte COMMAND_ANALOG_WRITE, byte codename, byte value;
digitalWrite (3):
	byte COMMAND_DIGITAL_WRITE, byte codename, byte value;
analogRead (2):
	byte COMMAND_ANALOG_READ, byte codename;
analogRead _return_ (3):
	byte COMMAND_ANALOG_READ_RE, int value;
digitalRead (2):
	byte COMMAND_DIGITAL_READ, byte codename;
digitalRead _return_ (3):
	byte COMMAND_DIGITAL_READ_RE, byte value;
ReadPot(2):
	byte COMMAND_READ_POT, byte codename;
ReadPot _return(3):
	byte COMMAND_READ_POT_RE, int value;
ReadKey(1):
	byte COMMAND_READ_KEY;
ReadKey _return(3):
	byte COMMAND_READ_KEY_RE, int value;

*/


/*
A message structure for Botton Bord will be:
switch mode (2):
	byte COMMAND_SWITCH_MODE, byte mode
run (5):
	byte COMMAND_RUN, int speedL, int speedR
analogWrite (3):
	byte COMMAND_ANALOG_WRITE, byte codename, byte value;
digitalWrite (3):
	byte COMMAND_DIGITAL_WRITE, byte codename, byte value;
analogRead (2):
	byte COMMAND_ANALOG_READ, byte codename;
analogRead _return_ (3):
	byte COMMAND_ANALOG_READ_RE, int value;
digitalRead (2):
	byte COMMAND_DIGITAL_READ, byte codename;
digitalRead _return_ (3):
	byte COMMAND_DIGITAL_READ_RE, int value;
read IR (1):
	byte COMMAND_READ_IR;
read IR _return_ (11):
	byte COMMAND_READ_IR_RE, int valueA, int valueB, int valueC, int valueD, int valueE;
read Trim (1);
  byte COMMAND_READ_TRIM;        
Read Trim _return(3):
	byte COMMAND_READ_TRIM_RE, int value;

*/

//************************************************************************************

struct EEPROM_BMP{
	char name[8];
	uint8_t width;
	uint8_t height;
	uint16_t address;
};


class RobotControl:public EEPROM_I2C, public Compass, public Arduino_LCD{

	public:
		RobotControl();
		void begin();
		void setMode(uint8_t mode);
		
		//Read & Write, TK0 - TK7, TKD0 - TKD1, bottom TK0 - TK4
		bool digitalRead(uint8_t port);
		int analogRead(uint8_t port);
		void digitalWrite(uint8_t port, bool value);
		void analogWrite(uint8_t port, uint8_t value);//It's not available, as there's no pin can be used for analog write
		
		//IR sensors from the bottom board
		//define an array as "int arr[4];", and supply the arry name here
		uint16_t IRarray[5];
		void updateIR();
		
		//on board Potentiometor
		int knobRead();
		//Potentiometor of the motor board
		int trimRead();
		
		//on board piezo
		void playFile_P(const char* filename_P, byte check_isPlAY);
		void playFile(char* filename, byte check_isPlAY);
		void playFileTime_P(const char* filename_P, int Tsec, byte check_isPlAY);
		void playFileTime(char* filename, int Tsec, byte check_isPlAY);
		void stopPlayFile();
		void pausePlayFile();
		void playFileDisable();
		void setPlayVolume(byte vol);
		void changePlayVolume(byte vol);
		void setPlayQuality(byte qual);
		void playFileLoop(byte set);
		boolean isPlaying();
		boolean playSoundBuzz(byte nSound);


		//WS2812
		void leds_Show(uint8_t style=SHOW_ALL_STYLES_UP, uint8_t ncycleInStyle=1, uint8_t ncycleAllStyles=255, uint16_t wait=50);
		uint8_t leds_Is_Show();
		void leds_Stop();
		void leds_Pause_Show();

		//compass
		uint16_t compassRead();
		void drawCompass(uint16_t value, uint8_t x, uint8_t y);
		void drawBase();
		void drawDire(int16_t dire);
		
		//keyboard
		void keyboardCalibrate(int *vals);
		int8_t keyboardRead();//return the key that is being pressed?Has been pressed(with _processKeyboard)?
		int analogReadKey();
		
		//movement
		void moveForward(int speed);
		void moveBackward(int speed);
		void turnLeft(int speed);
		void turnRight(int speed);
		void motorsStop();
		void motorsWritePct(int speedLeftPct, int speedRightPct);
		
		void motorsWrite(int speedLeft,int speedRight);
		void pointTo(int degrees);//turn to an absolute angle from the compass
		void turn(int degress);//turn certain degrees from the current heading
		
		//Line Following
		void lineFollowConfig(uint8_t KP, uint8_t KD, uint8_t robotSpeed, uint8_t intergrationTime);//default 11 5 50 10
		
		//TFT LCD
		//use the same commands as Arduino_LCD
		void beginTFT(uint16_t foreGround=BLACK, uint16_t background=WHITE);
		//void text(int value, uint8_t posX, uint8_t posY, bool EW);
		//void text(long value, uint8_t posX, uint8_t posY, bool EW);
		//void text(char* value, uint8_t posX, uint8_t posY, bool EW);
		//void text(char value, uint8_t posX, uint8_t posY, bool EW);
		void debugPrint(long value, uint8_t x=0, uint8_t y=0);
		void clearScreen();
		
		void drawBMP(char* filename, uint8_t x, uint8_t y); //detect if draw with EEPROM or SD, and draw it
		void drawBMP_P(const char* filename_P, uint8_t x, uint8_t y); //detect if draw with EEPROM or SD, and draw it.Filename stored im progr. memory
		void _drawBMP(uint32_t iconOffset, uint8_t x, uint8_t y, uint8_t width, uint8_t height); //draw from EEPROM
		int8_t _drawBMP(char* filename, uint8_t x, uint8_t y); //draw from SD
		int8_t _drawBMP_P(const char* filename_P, uint8_t x, uint8_t y); //draw from SD.Filename stored im progr. memory
		void beginBMPFromEEPROM();
		void endBMPFromEEPROM();
		bool Is_EEPROM_BMP_Allocated();
		void printEPROM_Structure();
		void displayLogos();
		void PlayFileGP(uint8_t nStr); //read and run file GP from SD. Filename stored im progr. memory
		//void PlayFileGP(char* filename); //read and run file GP from SD.
		
		uint16_t foreGround;	//foreground color
		uint16_t backGround;	//background color

		
		//SD card
		void beginSD();
		
		//Information in EEPROM
		void userNameRead(char* container);
		void robotNameRead(char* container);
		void cityNameRead(char* container);
		void countryNameRead(char* container);
		
		void userNameWrite(char* text);
		void robotNameWrite(char* text);
		void cityNameWrite(char* text);
		void countryNameWrite(char* text);
		
		//Others
		bool isActionDone();
		void pauseMode(uint8_t onOff);
		void waitContinueKey(uint8_t key=BUTTON_MIDDLE);
		void waitContinueKey_T(uint8_t key=BUTTON_MIDDLE);
		uint8_t waitContinueAnyKey_T();

		//PS2
		PS2X ps2x;			// create PS2 controller object
		int PS2ControlInit();
		void getPS2ControlInput();
		//void getPS2ControlInput_s();
		void GetOneJoystick();
		void GetTwoJoystick();

		//Ultrasonic HC-SR04
		uint16_t GetDistanceFr();
		uint8_t UScheckDistanceFr();
		uint16_t GetDistanceRv();
		uint8_t UScheckDistanceRv();

		//Wiskers GP2Y0A...
		uint8_t getWiskerLF();
		uint8_t getWiskerRF();
		uint8_t getWiskerLR();
		uint8_t getWiskerRR();
		static byte cntCheckWiskerLF;
		static byte cntCheckWiskerRF;
		static byte cntCheckWiskerLR;
		static byte cntCheckWiskerRR;
		static uint16_t UDistanceFr;
		static uint16_t UDistanceRv;
		uint32_t old_USDelayFr;
		uint32_t old_USDelayRv;


		//Lightt
		void checkLight();
		void FrontLightOnOff(bool setLight);
		bool F_lightON;
		static bool F_enableCheckLight_1s;
		byte countCheckLight;
		
		//Batarry voltage
		void checkVoltage();
		uint8_t countVBat;
		static bool F_enableCheckVBat_1s;

	private:
		//Read & Write
		uint8_t _getTypeCode(uint8_t port);//different ports need different actions
		
		bool _requestDigitalReadTop(uint8_t port);
		bool _requestDigitalReadBot(uint8_t port);

		int _requestAnalogReadTop(uint8_t port);
		int _requestAnalogReadBot(uint8_t port);

		void _requestDigitalWriteTop(uint8_t port, uint8_t value);
		void _requestDigitalWriteBot(uint8_t port, uint8_t value);
				
		void _requestAnalogWriteTop(uint8_t port, uint8_t value);

		//LCD
		void _enableLCD();
		void _setWrite(uint8_t posX, uint8_t posY);
		void _setErase(uint8_t posX, uint8_t posY);
		
		
		//SD
		//SdCard card;
		//Fat16 file;
		//Fat16 melody;
		File file;
		void _enableSD();

		//keyboard
		int averageAnalogInput(void);

		//Communication
		EasyTransfer2 messageOut1;
		EasyTransfer2 messageIn1;
		
		EasyTransfer2 messageOut2;
		EasyTransfer2 messageIn2;


		//TFT LCD
		bool _isEEPROM_BMP_Allocated;
		EEPROM_BMP * _eeprom_bmp;
		void _drawBMP_EEPROM(uint16_t address, uint8_t width, uint8_t height);
		void _drawBMP_SD(char* filename, uint8_t x, uint8_t y);

};

extern RobotControl Robot;
extern Ultrasonic ultrasonicFr;
extern Ultrasonic ultrasonicRv;
extern OneWire DS_18B20;


#endif
