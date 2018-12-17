#include "ArduinoRobot.h"

void RobotControl::beginSD(){

  const int chipSelect = 53;  // SS_PIN for Mega
  //card.init();
  //file.init(&card);
  //melody.init(&card);
  SD.begin(chipSelect);
}
//Mega2560 board
void RobotControl::_enableSD(){
  DDRL = DDRL & B11111101; //pinMode(LCD_CS,INPUT);   PL1
  DDRB = DDRB | B00000001; //pinMode(CARD_CS,OUTPUT); PB0
}

/*
void RobotControl::sdTest(){
  file.open("Infor.txt",O_READ);
  uint8_t buf[7];
  char n;
  while ((n = file.read(buf, sizeof(buf))) > 0) {
    for (uint8_t i = 0; i < n; i++) Serial.write(buf[i]);
  }
}*/
