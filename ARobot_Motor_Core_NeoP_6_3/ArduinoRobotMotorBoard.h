#ifndef ArduinoRobot_h
#define ArduinoRobot_h

#include "EasyTransfer2.h"
#include "Multiplexer.h"
#include "LineFollow.h"
#include "Adafruit_NeoPixel.h"

//#include "IRremote.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

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
#define COMMAND_PLAY_SAUND			125


//component codename
#define CN_LEFT_MOTOR	0
#define CN_RIGHT_MOTOR	1
#define CN_IR			2

//port types, for R/W
#define TYPE_B_TKD		0 
#define TYPE_B_TK		1
#define TYPE_B_SENSE	2

//motor board modes
#define MODE_SIMPLE			0
#define MODE_LINE_FOLLOW	1
#define MODE_ADJUST_MOTOR	2
#define MODE_IR_CONTROL		3

//bottom TKDs, just for communication purpose
#define B_TKD1		201
#define B_TKD2		202
#define B_TKD3		203 // NEOP, D4
#define B_TKD4		204	// ORS_ON, D12
#define B_SENSE_A	205 // CSA 
#define B_SENSE_B	206 // CSB

//bottom IRs, Multiplexer for communication purpose
#define B_IR0		210
#define B_IR1		211
#define B_IR2		212
#define B_IR3		213
#define B_IR4		214
#define B_VBAT		215 // R1=10k, R2=5,6k

//leds definitions
#define LED_PIN		4
#define LED_COUNT	16

#define RAINBOW				1

#define CYLON_UP			2
#define CYLON_DOUN			3
#define CYLON_UP_DOUN		4

#define CASCADE_UP			5
#define CASCADE_DOUN		6
#define CASCADE_UP_DOUN		7

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

#define MAX_STYLES					42

#define C_RED		0
#define C_GREEN		1
#define C_BLUE		2
#define C_WHITE		3
#define C_RED1		4
#define C_GREEN1	5
#define C_BLUE1		6
#define C_WHITE1	7

#define MAX_FOR_SENSE	320
#define MAX_TIME_SENSE	300


#define DEBUG


/*
A message structure will be:
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

class RobotMotorBoard:public LineFollow { 
	public:
		RobotMotorBoard();
		void begin();
		void process();
		void parseCommand();
		int  IRread(uint8_t num);
		void setMode(uint8_t mode);
		void pauseMode(bool onOff);
		void motorsWrite(int speedL, int speedR);
		void motorsWritePct(int speedLpct, int speedRpct);//write motor values in percentage
		void motorsStop();
		void leds_show(uint8_t style, uint8_t ncycleInStyle, uint8_t ncycleAllStyles, uint16_t wait);


		//WS2812 NeoPixel
		void beginNeoPixel();
		void runRainbow();
		void runCylon();
		void runCascade();
		void runColorWipe_W();
		void runColorWipe_R();
		void runColorWipe_G();
		void runColorWipe_B();
		void runColorWipe_RGBW();
		void runTheaterChase_W();
		void runTheaterChase_R();
		void runTheaterChase_G();
		void runTheaterChase_B();
		void runTheaterChase_RGBW();
		void runRainbow1();
		void runRainbowCycle();
		void runTheaterChaseRainbow();
		void clearLEDs();


	private:
		
		//convert codename to actual pins
		uint8_t parseCodename(uint8_t codename);
		uint8_t codenameToAPin(uint8_t codename);
		
		void stopCurrentActions();
		void _config();
		//void sendCommand(byte command,byte codename,int value);
		
		void _analogWrite(uint8_t codename, int value);
		void _digitalWrite(uint8_t codename, bool value);
		void _analogRead(uint8_t codename);
		void _analogReadBotPin(uint8_t codename);
		void _analogReadBotMux(uint8_t codename);
		uint8_t _portToBotMux(uint8_t codename);
		uint8_t _getTypeCode(uint8_t codename);
		void _digitalRead(uint8_t codename);
		void _readIR();
		void _readTrim();
		void _leds_show();
		void _leds_stop();
		void _leds_pause();
		void _leds_pause_show();

		void _refreshMotorAdjustment();
		
		Multiplexer IRs;
		uint8_t mode;
		uint8_t isPaused;
		EasyTransfer2 messageIn;
		EasyTransfer2 messageOut;
		
		//Line Following 
		void reportActionDone();
		float motorAdjustment;//-1.0 ~ 1.0, whether left is lowered or right is lowered

		//SENSE
		void checkSense();

		//WS2812 NeoPixel
		void _initForShow(uint8_t style, uint8_t ncycle, uint16_t wait);
		void _initForShowAll();
		void _cylon(uint32_t color);
		void _cascade(uint32_t color);
		void _rainbowBat(byte startPosition);
		void _rainbow(void);
		void _colorWipe(uint32_t color);
		uint32_t _rainbowOrder(byte position);
		void _theaterChase(uint32_t c);
		void _theaterChaseRainbow();
		void _rainbow1();
		void _rainbowCycle();
		uint32_t _Wheel(byte WheelPos);
		void _leds_Is_Show();
		uint8_t _getStyleRandom(uint8_t maxNumStyles);
		bool _checkIfAllCycEnd();
		void _initFlagsForLedsShow();
		void _setForShowAll();


		bool F_StartDisplay, F_Show_cont, F_show_up, F_clearLEDs, F_AllStyleRandom, F_AllStyle_Up, F_showAllStyles;
		bool F_color_Wipe_RGBW, F_show_Up_Down, F_theaterChase_RGBW, F_sense, F_stopMot, F_Show_contAllStyles;
		bool F_pause; bool F_stop;
		uint8_t _style, _style_s, _ncycle, _ncycle_s, _color, _ncycleAllStyles, _ncycleAllStyleRandom;
		int16_t _i_pos, _j_pos;
		uint16_t _wait, _wait_s;
		uint32_t TimerDelay, senseTime_old;


};

extern RobotMotorBoard RobotMotor;
extern Adafruit_NeoPixel leds;

#endif
