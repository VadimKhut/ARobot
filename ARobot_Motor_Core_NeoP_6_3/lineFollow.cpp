//#include <ArduinoRobotMotorBoard.h>
#include "LineFollow.h"

//#define KP 19 //0.1 units
//#define KD 14
//#define ROBOT_SPEED 100 //percentage

//#define KP 11
//#define KD 5
//#define ROBOT_SPEED 50

//#define INTEGRATION_TIME 10 //En ms

/*uint8_t KP=11;
uint8_t KD=5;
uint8_t robotSpeed=50; //percentage
uint8_t intergrationTime=10;*/

#define LEVEL_LINE 50 

/*int readIR_sensor[5], last_error=0, acu=0;

//Estos son los arrays que hay que rellenar con los valores de los sensores
//soil on white and black. 
int sensor_white[]={0,0,0,0,0};
int sensor_black[]={1023,1023,1023,1023,1023};
*/

//unsigned long time;

//void mueve_robot(int vel_izq, int vel_der);
//void para_robot();
//void doCalibration(int speedPct, int time);
//void adjust_levels();	//calibrate values


#define DEBUG


LineFollow::LineFollow(){

	/*
	KP=11;
	KD=5;
	robotSpeed=50; //percentage
	intergrationTime=10;
	*/
	config(11,5,50,10);
	//init for white and black data
	for(int i=0; i < 5; i++){
		sensor_white[i]=994;
		sensor_black[i]=120;
	}
}



void LineFollow::config(uint8_t KP, uint8_t KD, uint8_t robotSpeed, uint8_t intergrationTime){

	this->KP=KP;
	this->KD=KD;
	this->robotSpeed=robotSpeed;
	this->intergrationTime=intergrationTime;
	/*Serial.print("LFC: ");
	Serial.print(KP);
	Serial.print(' ');
	Serial.print(KD);
	Serial.print(' ');
	Serial.print(robotSpeed);
	Serial.print(' ');
	Serial.println(intergrationTime);*/
}


void LineFollow::calibIRs(){
	static bool isInited=false;//So only init once

	if(isInited)
		return ;
	
	delay(1000);
	
	doCalibration(51,500);
	doCalibration(-51,800);
	doCalibration(51,500);

#ifdef DEBUG
	Serial.println(F("sensor_white: "));
	for(int i=0; i < 5; i++){
		Serial.print(sensor_white[i], DEC);
		Serial.print(F("  "));
	}
	Serial.println();
	Serial.println(F("sensor_black: "));
	for(int i=0; i < 5; i++){
		Serial.print(sensor_black[i], DEC);
		Serial.print(F("  "));
	}
#endif
	delay(1000);
	isInited=true;
}


void LineFollow::runLineFollow(){
	int motor_left;
	int motor_right;
	int speed;
	int error;

	for(int count=0; count < 5; count++){

		readIR_sensor[count]=map(IRread(count),sensor_black[count],sensor_white[count],0,127);
		acu+=readIR_sensor[count];
	}

	//Serial.println(millis());
	if (acu > LEVEL_LINE){
		acu/=5;

		error = ((readIR_sensor[0]<<6)+(readIR_sensor[1]<<5)-(readIR_sensor[3]<<5)-(readIR_sensor[4]<<6))/acu;

		error = constrain(error,-100,100);

		//calculate the speed correction using a filter PD
		speed = (error * KP)/10 + (error-last_error)*KD;

		last_error = error;

		//Correct forward speed with error filter output PD
		motor_left=constrain((robotSpeed + speed),-100,100);
		motor_right =constrain((robotSpeed - speed),-100,100);

		//Move the robot
		motorsWritePct(motor_left,motor_right);

		//wait for the reaction of the robot
		delay(intergrationTime);
	}
	else{ //(acu <= LEVEL_LINE) // 
		//found a black line perpendicular to way
		//we stopped the robot
		motorsStop();
		reportActionDone();
		//setMode(MODE_SIMPLE);
	}
}  


void LineFollow::doCalibration(int speedPct, int time){
	unsigned long beginTime;

	motorsWritePct(speedPct, -speedPct);

	beginTime = millis();
	while((millis()-beginTime) < time)
		adjust_levels();
	motorsStop();
}


void LineFollow::adjust_levels(){
	int readIR=0;

	for(int count=0; count < 5; count++){
		readIR=IRread(count);

		if (readIR > sensor_white[count])
			sensor_white[count]=readIR;

		if (readIR < sensor_black[count])
			sensor_black[count]=readIR;
	} 
}






