#include "ArduinoRobotTopBoard.h"
#include "SD.h"
#include "TMRpcm.h"





#define  F5000HZ      231       //5000HZ=0.2ms/2 = 0.1ms;256-25=D'231';
#define  F4100HZ      226       //4100HZ=0.22ms/2 = 0.12ms;256-30=D'226';
#define  F400HZ       236       //400HZ=0.5ms/2 = 0.25ms (Timer2_start_BUZZ_Low)
#define  F600HZ       243       //600HZ=0.8ms/2 = 0.4ms (Timer2_start_BUZZ_Low)
#define  F900HZ       248       //932HZ (Timer2_start_BUZZ_Low)


volatile byte _tcnt2;
volatile byte F_Buzz;
bool F_startBuzzDurat=false;
bool F_pause_tmrpcm;
uint16_t _Buzzdur;
uint32_t TimerDelayBuzz;






void RobotTopBoard::PlaySoundBuzz(uint8_t nSound){

	switch(nSound){
		case 1:
			Play_Buzz(50,F5000HZ);
			break;
		case 2:
			Play_Buzz(50,F4100HZ);
			break;
		case 3:
			Play_Buzz_Low(400, F600HZ);
			break;
		case 4:
			Play_Buzz_Low(400, F900HZ);
			break;
		case 5:
			Play_Buzz(100, F4100HZ);
			break;
		case 6:
			Play_Buzz(250,F5000HZ); 
			break;
		case 7:
			Play_Buzz(100,F5000HZ);
			break;
	}
}





//**********************************************************************  
//  Play_Buzz(100, F5000HZ);    //short hight pitch beep (beep_butons)
//  Play_Buzz(250, F5000HZ);    //long hight pitch beep (beep)
//  Play_Buzz(100, F4100HZ);    //sound for driver revers (beep_rev)

//  Play_Buzz_Low(400, F900HZ); //sound_bad,  (Timer4_start_BUZZ_Low)
//  Play_Buzz_Low(600, F600HZ); //            (Timer4_start_BUZZ_Low)
//**********************************************************************  
void RobotTopBoard::Play_Buzz(uint16_t duration, uint8_t freq){

	if(tmrpcm.isPlaying() && !tmrpcm.isPause()){
		tmrpcm.pause();
		F_pause_tmrpcm=true;	//flag pause for play buzzer sound
		_play_Buzz(duration,freq);
	}
	else
		_play_Buzz(duration,freq);
}


void RobotTopBoard::_play_Buzz(uint16_t duration, uint8_t freq){

	Timer4_start_BUZZ(freq);
	F_startBuzzDurat=true;
	_Buzzdur=duration;
	TimerDelayBuzz=millis();  // reset TimerDelay
}



void RobotTopBoard::Play_Buzz_Low(uint16_t duration, uint8_t freq){

	if(tmrpcm.isPlaying() && !tmrpcm.isPause()){
		tmrpcm.pause();			//stop play
		F_pause_tmrpcm=true;	//flag pause for play buzzer sound
		_play_Buzz_Low(duration,freq);
	}
	else
		_play_Buzz_Low(duration,freq);
}


void RobotTopBoard::_play_Buzz_Low(uint16_t duration, uint8_t freq){

	Timer4_start_BUZZ_Low(freq);
	F_startBuzzDurat=true;
	_Buzzdur=duration;
	TimerDelayBuzz=millis();  // reset TimerDelay
}





void RobotTopBoard::DoBuzzDuration(){

	uint32_t TimerDelayBuzz_t = millis();
	if ((TimerDelayBuzz_t - TimerDelayBuzz > _Buzzdur)){
		F_startBuzzDurat=false;
		Timer4_stop();
		digitalWrite(BUZZ_2,LOW);			//set low, does not ring a buzzer
		if(F_pause_tmrpcm){
			tmrpcm.pause();					//continue play
			F_pause_tmrpcm=false;
		}
		messageOut.writeByte(COMMAND_PLAY_BUZZ_RE);
		messageOut.writeByte(1);
		messageOut.sendData();	
	}
}




void RobotTopBoard::Timer4_start_BUZZ(uint8_t freq) {
	TCCR4B=0;
	TCCR4A=0;
	TCCR4C=0;
	TCCR4D=0;
	TCCR4E=0;

	_tcnt2=freq;
	TCNT4=_tcnt2;												//set the TMR4 initial value
	TCCR4B= _BV(CS42) | _BV(CS41) | _BV(CS40) | _BV(PSR4);		//clk/64(from pescacing)
	TIFR4=_BV(TOV4);											//reset flag TOV4
	TIMSK4=_BV(TOIE4);											//allow TMR4 interrupt
}


void RobotTopBoard::Timer4_start_BUZZ_Low(uint8_t freq) {
	TCCR4B=0;
	TCCR4A=0;
	TCCR4C=0;
	TCCR4D=0;
	TCCR4E=0;

	_tcnt2=freq;
	TCNT4=_tcnt2;												//set the TMR4 initial value
	TCCR4B=_BV(CS43) | _BV(CS41) |_BV(CS40) | _BV(PSR4);		//clk/1024(from pescacing)
	TIFR4=_BV(TOV4);											//reset flag TOV4
	TIMSK4=_BV(TOIE4);											//allow TMR4 Overflow interrupt
}



void RobotTopBoard::Timer4_stop() {
	TIMSK4=0;													//TMR4 Overflow Interrupt Disable
}



void RobotTopBoard::Timer4_overflow_BUZZ() {
	TCNT4=_tcnt2;												//To the timer timing initial value
	F_Buzz=~F_Buzz;
	if(F_Buzz)
		digitalWrite(BUZZ_2,HIGH);								//set high, the buzzer
	else
		digitalWrite(BUZZ_2,LOW);								//set low, does not ring a buzzer
}



//****** TIMER4_OVF *************************************************      
ISR(TIMER4_OVF_vect) {
	RobotTop.Timer4_overflow_BUZZ();
}










//void RobotTopBoard::playSound(uint8_t nSound){
//
//	switch(nSound){
//		case 1:
//			MSound( 1, 50, 2000);
//			break;
//		case 2:
//			MSound(3, 60, 2000, 80, 2250, 100, 2500);
//			break;
//		case 3:
//			MSound(3, 100, 2500, 80, 2250, 60, 2000);
//			break;
//		case 4:
//			MSound( 1, 45, 2000);
//			break;
//		case 5:
//			MSound (2, 40, 2500, 40, 2500);
//			break;
//		case 6:
//			MSound (2, 50, 2000, 50, 2000); 
//			break;
//		case 7:
//			MSound( 1, 50, 6000);
//			break;
//	}
//}
//



////-------------------------------------------------------------------------------
////    SoundNoTimer - Quick and dirty tone function to try to output a frequency
////            to a speaker for some simple sounds.
////-------------------------------------------------------------------------------
//
//  void RobotTopBoard::_SoundNoTimer(unsigned long duration,  unsigned int frequency){
//    long toggle_count = 0;
//    long lusDelayPerHalfCycle;
// 
//    toggle_count = 2 * frequency * duration / 1000;
//    lusDelayPerHalfCycle = 1000000L/(frequency * 2);
//  
//    // if we are using an 8 bit timer, scan through prescalars to find the best fit
//    while (toggle_count--) {
//      // toggle the pin
//      *pin_port ^= pin_mask;
//  
//      // delay a half cycle
//      delayMicroseconds(lusDelayPerHalfCycle);
//    }    
//    *pin_port &= ~(pin_mask);  // keep pin low after stop
//  }
//  
//  void RobotTopBoard::MSound(byte cNotes, ...){
//    va_list ap;
//    unsigned int uDur;
//    unsigned int uFreq;
//    va_start(ap, cNotes);
//  
//    while (cNotes > 0) {
//      uDur = va_arg(ap, unsigned int);
//      uFreq = va_arg(ap, unsigned int);
//      _SoundNoTimer(uDur, uFreq);
//      cNotes--;
//    }
//    va_end(ap);
//  }
