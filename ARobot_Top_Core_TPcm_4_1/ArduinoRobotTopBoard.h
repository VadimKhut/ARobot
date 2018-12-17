#ifndef ArduinoRobotTopBoard_h
#define ArduinoRobotTopBoard_h

#include "EasyTransfer2.h"
#include "Multiplexer.h"
#include "TMRpcm.h"
#include "SD.h"
//#include "IRremote.h"
#include "Arduino.h"


//Command code
#define COMMAND_ANALOG_WRITE		20
#define COMMAND_DIGITAL_WRITE		30
#define COMMAND_ANALOG_READ			40
#define COMMAND_ANALOG_READ_RE		41
#define COMMAND_DIGITAL_READ		50
#define COMMAND_DIGITAL_READ_RE		51
#define COMMAND_READ_POT			82
#define COMMAND_READ_POT_RE			83
#define COMMAND_ANALOG_READ_KEY		84
#define COMMAND_ANALOG_READ_KEY_RE	85
#define COMMAND_AVERAGE_READ_KEY	86
#define COMMAND_AVERAGE_READ_KEY_RE	87
#define COMMAND_CALIBRATE_KEY		88
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



#define NUMBER_BUTTONS	5
#define BUZZ	        5
#define BUZZ_2	        10

//port types, for R/W
#define TYPE_TOP_TK    0
#define TYPE_TOP_TKD   1


//top TKs Multiplexer
#define T_TK0   100
#define T_TK1   101
#define T_TK2   102
#define T_TK3   103
#define T_TK4   104
#define T_TK5   105
#define T_TK6   106
#define T_TK7   107
#define T_BCHRG 108
#define T_BON   109


//top TKDs, just for communication purpose
#define T_TKD0 116
#define T_TKD1 117
#define T_TKD2 118
#define T_TKD3 119
#define T_TKD4 120
#define T_TKD5 121
#define T_LED1 122


#define IGNORE_isPlAY	0
#define SKEEP_isPlAY	1

/*
A message structure will be:
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

class RobotTopBoard{
	public:
		RobotTopBoard();
		void begin();
		void process();
		void parseCommand();

		//on board buzzer
		char ArStr[35];  //Array for any script
		void PlaySoundBuzz(uint8_t nSound); 
		void DoBuzzDuration();
		void Timer4_start_BUZZ_Low(uint8_t freq);
		void Timer4_start_BUZZ(uint8_t freq);
		void Timer4_stop();
		void Timer4_overflow_BUZZ();
		void Play_Buzz(uint16_t duration, uint8_t freq);
		void Play_Buzz_Low(uint16_t duration, uint8_t freq);
		void _play_Buzz(uint16_t duration, uint8_t freq);
		void _play_Buzz_Low(uint16_t duration, uint8_t freq);

		byte nSound;
		int Tsec;

		//SD card
		void beginSD();


	private:
		//convert codename to actual pins
		uint8_t parseCodename(uint8_t codename);
		uint8_t codenameToAPin(uint8_t codename);
		void _analogWrite(uint8_t codename, int value);
		void _digitalWrite(uint8_t codename, bool value);
		void _analogRead(uint8_t codename);
		void _digitalRead(uint8_t codename);
		uint8_t _codenameToPin(uint8_t codename);
		uint8_t _codenameToAPin(uint8_t codename);
		uint8_t _getTypeCode(uint8_t codename);
		uint8_t _portToTopMux(uint8_t codename);
		void _digitalReadTopMux(uint8_t codename);
		void _digitalReadTopPin(uint8_t codename);
		void _analogReadTopMux(uint8_t codename);
		void _analogReadTopPin(uint8_t codename);
		void _readKeybord();
		void _readKnob();
		void _averageReadKeybord();
		int8_t _averageKeyboardRead();
		void _keybordCalibrate();
		void _playFile();
		void _playFileTime();
		void _playFileLoop();
		void _stopPlayback();
		void _disablePlay();
		void _pausePlay();
		void _isPlaying();
		void _setVolume();
		void _changeVolume();
		void _setQuality();
		void _playSoundBuzz();
		void _SoundNoTimer(unsigned long duration,  unsigned int frequency);

		
		//SD
		TMRpcm tmrpcm;

		Multiplexer Mult;
		EasyTransfer2 messageIn;
		EasyTransfer2 messageOut;
};

extern RobotTopBoard RobotTop;

#endif
