#include "ArduinoRobot.h"
#include "Wire.h"

RobotControl::RobotControl():Arduino_LCD(LCD_CS,DC_LCD,RST_LCD){

}

void RobotControl::begin(){
	Wire.begin();
	//Compass
	//nothing here
		
	//communication
	Serial1.begin(9600);
	messageOut1.begin(&Serial1);
	messageIn1.begin(&Serial1);

	Serial2.begin(9600);
	messageOut2.begin(&Serial2);
	messageIn2.begin(&Serial2);



	//TFT initialization
	//Arduino_LCD::initR(INITR_GREENTAB);
}

void RobotControl::setMode(uint8_t mode){
	messageOut2.writeByte(COMMAND_SWITCH_MODE);
	messageOut2.writeByte(mode);
	messageOut2.sendData();
}


RobotControl Robot=RobotControl();
Ultrasonic ultrasonicFr = Ultrasonic(TRIG_FR,ECHO_FR);
Ultrasonic ultrasonicRv = Ultrasonic(TRIG_RV,ECHO_RV);
OneWire DS_18B20 = OneWire(DATA_DS18B20);