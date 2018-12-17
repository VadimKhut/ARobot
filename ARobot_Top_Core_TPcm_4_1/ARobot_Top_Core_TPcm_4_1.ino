/* Motor Core


  This code for the Arduino Robot's motor board
  is the stock firmware. program the motor board with 
  this sketch whenever you want to return the motor
  board to its default state.
  
*/


#include "ArduinoRobotTopBoard.h"

void setup(){

	RobotTop.begin();
	RobotTop.beginSD();
}


void loop(){
	RobotTop.parseCommand();
	RobotTop.process();
}
