/* SparkFun WS2812 Breakout Board Example
  SparkFun Electronics
  date: July 25, 2013
  license: GNU GENERAL PUBLIC LICENSE
  
  Requires the Adafruit NeoPixel library. It's awesome, go get it.
  https://github.com/adafruit/Adafruit_NeoPixel
  
  This simple example code runs three sets of animations on a group of WS2812
  breakout boards. The more boards you link up, the better these animations
  will look. 
  
  For help linking WS2812 breakouts, checkout our hookup guide:
  https://learn.sparkfun.com/tutorials/ws2812-breakout-hookup-guide
  
  Before uploading the code, make sure you adjust the two defines at the
  top of this sketch: PIN and LED_COUNT. Pin should be the Arduino pin
  you've got connected to the first pixel's DIN pin. By default it's
  set to Arduino pin 4. LED_COUNT should be the number of breakout boards
  you have linked up.
*/

#include "ArduinoRobotMotorBoard.h"
#include "Adafruit_NeoPixel.h"
#include "WS2812_Definitions.h"


#define DEBUG

// Create an instance of the Adafruit_NeoPixel class called "leds".
// That'll be what we refer to from here on...
// Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void RobotMotorBoard::beginNeoPixel(){
	
	leds.begin();  // Call this to start up the LED strip.
  	clearLEDs();   // This function, defined below, turns all LEDs off...
  	leds.show();   // ...but the LEDs don't actually update until you call this.
}


// Ride the Rainbow Road
void RobotMotorBoard::runRainbow(){

	if(_ncycle > 0){
		_rainbow();
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{ // F_showAllStyles == false
		_style=0;		// stop show leds
		F_stop=true;
	#ifdef DEBUG
		Serial.print(F("_style= "));
		Serial.println(_style);
	#endif
		}
		clearLEDs();
		leds.show();
	}
}
  

void RobotMotorBoard::_rainbow(void){

	uint32_t TimerDelay_t = millis();
	if ((TimerDelay_t - TimerDelay > _wait) || F_StartDisplay){ // Delay for visibility
		F_StartDisplay = false;
		if (_i_pos < LED_COUNT*10){
			_rainbowBat(_i_pos);
			_i_pos++;
			TimerDelay = millis();  // reset TimerDelay
		}
		else{	// end one cycle
			if(!F_Show_cont || F_showAllStyles)
				_ncycle--;
		#ifdef DEBUG
			Serial.print(F("ncyc= "));
			Serial.println(_ncycle);
		#endif
			_i_pos=0;
		}
	}
}




// Prints a rainbow on the ENTIRE LED strip.
// The rainbow begins at a specified position. 
// ROY G BIV!
void RobotMotorBoard::_rainbowBat(byte startPosition){
	// Need to scale our rainbow. We want a variety of colors, even if there
	// are just 10 or so pixels.
	int rainbowScale = 192 / LED_COUNT;

	// Next we setup each pixel with the right color
	for (int i=0; i < LED_COUNT; i++){
		// There are 192 total colors we can get out of the rainbowOrder function.
		// It'll return a color between red->orange->green->...->violet for 0-191.
		leds.setPixelColor(i, _rainbowOrder((rainbowScale * (i + startPosition)) % 192));
	}
	// Finally, actually turn the LEDs on:
	leds.show();
}



// Input a value 0 to 191 to get a color value.
// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
// Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
uint32_t RobotMotorBoard::_rainbowOrder(byte position){
	// 6 total zones of color change:
	if (position < 31){  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
		return leds.Color(0xFF, position * 8, 0);
	}else if (position < 63){  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
		position -= 31;
		return leds.Color(0xFF - position * 8, 0xFF, 0);
	}else if (position < 95){  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
		position -= 63;
		return leds.Color(0, 0xFF, position * 8);
	}else if (position < 127){  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
		position -= 95;
		return leds.Color(0, 0xFF - position * 8, 0xFF);
	}else if (position < 159){  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
		position -= 127;
		return leds.Color(position * 8, 0, 0xFF);
	}else{  //160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
		position -= 159;
		return leds.Color(0xFF, 0x00, 0xFF - position * 8);
	}
}



// Indigo cylon
// Do a cylon (larson scanner)
void RobotMotorBoard::runCylon(){

	if (_ncycle > 0){
 		_cylon(INDIGO);  // Indigo cylon eye!
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		#ifdef DEBUG
			Serial.print(F("_style= "));
			Serial.println(_style);
		#endif
		}
		clearLEDs();
		leds.show();
	}
}  


// Implements a little larson "cylon" sanner.
// This'll run one full cycle, down one way and back the other
void RobotMotorBoard::_cylon(uint32_t color){
	// weight determines how much lighter the outer "eye" colors are
	const byte weight = 4;  
	// It'll be easier to decrement each of these colors individually
	// so we'll split them out of the 24-bit color value
	byte red = (color & 0xFF0000) >> 16;
	byte green = (color & 0x00FF00) >> 8;
	byte blue = (color & 0x0000FF);

	uint32_t TimerDelay_t = millis();
	if ((TimerDelay_t - TimerDelay > _wait) || F_StartDisplay){ // Delay for visibility
		F_StartDisplay = false;
		if(F_show_up){
		// Start at closest LED, and move to the outside
			if (_i_pos <= LED_COUNT-1){
				clearLEDs();
				leds.setPixelColor(_i_pos, red, green, blue);  // Set the bright middle eye
				// Now set two eyes to each side to get progressively dimmer
				for (int j=1; j<3; j++){
					if (_i_pos-j >= 0)
						leds.setPixelColor(_i_pos-j, red/(weight*j), green/(weight*j), blue/(weight*j));
					if (_i_pos-j <= LED_COUNT)
						leds.setPixelColor(_i_pos+j, red/(weight*j), green/(weight*j), blue/(weight*j));
				}
				leds.show();  // Turn the LEDs on
				_i_pos++;
				TimerDelay = millis();  // reset TimerDelay
			}
			else{	// end one cycle
				if(_style == CYLON_UP_DOUN){
					F_show_up = false;
					_i_pos=LED_COUNT-2;
				}
				else{	//CYLON_UP
					if(!F_Show_cont || F_showAllStyles){
						_ncycle--;
					#ifdef DEBUG
						Serial.print(F("ncyc= "));
						Serial.println(_ncycle);
					#endif
					}
					_i_pos=0;
				}
			}
		}
		else if(!F_show_up){	//(F_show_up == 0)
			// Now we go back to where we came. Do the same thing.
			if (_i_pos >= 1){
				clearLEDs();
				leds.setPixelColor(_i_pos, red, green, blue);
				for (int j=1; j<3; j++){
					if (_i_pos-j >= 0)
						leds.setPixelColor(_i_pos-j, red/(weight*j), green/(weight*j), blue/(weight*j));
					if (_i_pos+j <= LED_COUNT)
						leds.setPixelColor(_i_pos+j, red/(weight*j), green/(weight*j), blue/(weight*j));
				}
				leds.show();
				_i_pos--;
				TimerDelay = millis();  // reset TimerDelay
			}
			else{	// end one cycle
				if(_style == CYLON_UP_DOUN){
					F_show_up = true;
					if(!F_Show_cont || F_showAllStyles){
						_ncycle--;
					#ifdef DEBUG
						Serial.print(F("ncyc= "));
						Serial.println(_ncycle);
					#endif
					}
					_i_pos=0;
				}
				else{	// CYLON_DOUN
					if(!F_Show_cont || F_showAllStyles)
						_ncycle--;
					_i_pos=LED_COUNT-2;
				}
			}
		}
	}
}



// A light shower of spring green rain
// This will run the cascade from top->bottom
void RobotMotorBoard::runCascade(){

	if (_ncycle > 0){
		// First parameter is the color, second is direction, third is ms between falls
		_cascade(MEDIUMSPRINGGREEN); 
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
		_style=0;		// stop show leds
		F_stop=true;
	#ifdef DEBUG
		Serial.print(F("_style= "));
		Serial.println(_style);
	#endif
		}
		clearLEDs();
		leds.show();
	}
}


// Cascades a single direction. One time.
void RobotMotorBoard::_cascade(uint32_t color){

	uint32_t TimerDelay_t = millis();
	if ((TimerDelay_t - TimerDelay > _wait) || F_StartDisplay){ // Delay for visibility
		F_StartDisplay=false;

		if (F_show_up){
			if (_i_pos < LED_COUNT){
				clearLEDs();	// Turn off all LEDs
				leds.setPixelColor(_i_pos, color);	// Set just this one
				leds.show();
				_i_pos++;
				TimerDelay=millis();  // reset TimerDelay
			}
			else{
				if(_style == CASCADE_UP_DOUN){
					F_show_up=false;
					_i_pos=LED_COUNT-1;
				}
				else{	//CASCADE_UP
					if(!F_Show_cont || F_showAllStyles){
						_ncycle--;
					#ifdef DEBUG
						Serial.print(F("ncyc= "));
						Serial.println(_ncycle);
					#endif
					}
					_i_pos=0;
				}
			}
		}
		else if(!F_show_up){	//(F_show_up == 0)
			if (_i_pos >= 0){
				clearLEDs();	// Turn off all LEDs
				leds.setPixelColor(_i_pos, color);	// Set just this one
				leds.show();
				_i_pos--;
				TimerDelay = millis();  // reset TimerDelay
			}
			else{
				if(_style == CASCADE_UP_DOUN){
					F_show_up = true;
					if(!F_Show_cont || F_showAllStyles){
						_ncycle--;
					#ifdef DEBUG
						Serial.print(F("ncyc= "));
						Serial.println(_ncycle);
					#endif
					}
					_i_pos=0;
				}
				else{	// CASCADE_DOUN
					if(!F_Show_cont || F_showAllStyles){
						_ncycle--;
					#ifdef DEBUG
						Serial.print(F("ncyc= "));
						Serial.println(_ncycle);
					#endif
					}
					_i_pos = LED_COUNT-1;
				}
			}
		}
	}
}



// Sets all LEDs to off, but DOES NOT update the display;
// call leds.show() to actually turn them off after this.
void RobotMotorBoard::clearLEDs(){
	for (int i=0; i < LED_COUNT; i++){
		leds.setPixelColor(i, 0);
	}
}


// Some example procedures showing how to display to the pixels:
void RobotMotorBoard::runColorWipe_R(){

	if (_ncycle > 0){
		_colorWipe(leds.Color(255, 0, 0)); // Red
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::runColorWipe_G() {

	if (_ncycle > 0){
	  _colorWipe(leds.Color(0, 255, 0)); // Green
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
	#ifdef DEBUG
		Serial.print(F("_style= "));
		Serial.println(_style);
	#endif
	}
}


void RobotMotorBoard::runColorWipe_B() {

	if (_ncycle > 0){
	  _colorWipe(leds.Color(0, 0, 255)); // Blue
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::runColorWipe_W() {

	if (_ncycle > 0){
	  _colorWipe(leds.Color(127, 127, 127)); // White
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::runColorWipe_RGBW() {

	if (_ncycle > 0){
		if(_color == C_RED || _color == C_RED1){
			_colorWipe(leds.Color(255, 0, 0)); // Red
		}
		else if(_color == C_GREEN || _color == C_GREEN1){
			_colorWipe(leds.Color(0, 255, 0)); // Green
		}
		else if(_color == C_BLUE || _color == C_BLUE1){
			_colorWipe(leds.Color(0, 0, 255)); // Blue
		}
		else if(_color == C_WHITE || _color == C_WHITE1){
			_colorWipe(leds.Color(127, 127, 127)); // White
		}
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::_colorWipe(uint32_t color) {

	uint32_t TimerDelay_t = millis();
	if ((TimerDelay_t - TimerDelay > _wait) || F_StartDisplay){ // Delay for visibility
		F_StartDisplay=false;
		if (F_show_up){
			if(_i_pos < LED_COUNT){
			  leds.setPixelColor(_i_pos, color);
				_i_pos++;
				leds.show();
				TimerDelay = millis();  // reset TimerDelay
			}	// end one cycle
			else{
				if(!F_color_Wipe_RGBW){
					clearLEDs();
					leds.show();
					if(F_show_Up_Down){
						F_show_up=false;
						_i_pos=LED_COUNT-1;
					}	
					else{ // F_show_Up_Down == 0
						if(!F_Show_cont || F_showAllStyles){
							_ncycle--;
						#ifdef DEBUG
							Serial.print(F("ncyc= "));
							Serial.println(_ncycle);
						#endif
						}
					_i_pos=0;
					}
				}
				else if(F_color_Wipe_RGBW){ 
					if(F_show_Up_Down){
						F_show_up=false;
						_i_pos=LED_COUNT-1;
						//clearLEDs();
						//leds.show();
						_color++;
					}
					else{ // F_show_Up_Down == 0
						if(_color == 3){
							if(!F_Show_cont || F_showAllStyles){
								_ncycle--;
							#ifdef DEBUG
								Serial.print(F("ncyc= "));
								Serial.println(_ncycle);
							#endif
							}
						}
						_color++;
						_color %= 4;	//change color W,R,G,B
						_i_pos=0;
					}
				}
			}
		}
		else if (!F_show_up){	//(F_show_up == 0)
			if(_i_pos >= 0){
				leds.setPixelColor(_i_pos, color);
				_i_pos--;
				leds.show();
				TimerDelay = millis();  // reset TimerDelay
			}	// end one cycle
			else{
				if(!F_color_Wipe_RGBW){
					clearLEDs();
					leds.show();
					if(F_show_Up_Down){
						F_show_up=true;
						_i_pos=0;
					} // F_show_Up_Down == 0
					else{
						_i_pos = LED_COUNT-1;
					}	
					if(!F_Show_cont || F_showAllStyles){
						_ncycle--;
				#ifdef DEBUG
					Serial.print(F("ncyc= "));
					Serial.println(_ncycle);
				#endif
					}
				}
				else if(F_color_Wipe_RGBW){ 
					if(F_show_Up_Down){
						F_show_up=true;
						_i_pos=0;
					}
					else{ // F_show_Up_Down == 0
						_i_pos=LED_COUNT-1;
					}
					if(_color == 3 && !F_show_Up_Down){
						if(!F_Show_cont || F_showAllStyles){
							_ncycle--;
						#ifdef DEBUG
							Serial.print(F("ncyc= "));
							Serial.println(_ncycle);
						#endif
						}
					}
					else if(_color == 7 && F_show_Up_Down){
						if(!F_Show_cont || F_showAllStyles){
							_ncycle--;
						#ifdef DEBUG
							Serial.print(F("ncyc= "));
							Serial.println(_ncycle);
						#endif
						}
					}
					_color++;
					_color %= 8;	//change color W,R,G,B
				}
			}
		}
	}
}



// Send a theater pixel chase in...
void RobotMotorBoard::runTheaterChase_R() {

	if (_ncycle > 0){
		  _theaterChase(leds.Color(127, 0, 0)); // Red
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			_setForShowAll();
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::runTheaterChase_G() {

	if (_ncycle > 0){
	  _theaterChase(leds.Color(0, 255, 0)); // Green
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
		_style=0;		// stop show leds
		F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::runTheaterChase_B() {

	if (_ncycle > 0){
	  _theaterChase(leds.Color( 0, 0, 127)); // Blue
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::runTheaterChase_W() {

	if (_ncycle > 0){
		_theaterChase(leds.Color(127, 127, 127)); // White
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::runTheaterChase_RGBW() {

	if (_ncycle > 0){
		if(_color == C_RED || _color == C_RED1){
			_theaterChase(leds.Color(255, 0, 0)); // Red
		}
		else if(_color == C_GREEN || _color == C_GREEN1){
			_theaterChase(leds.Color(0, 255, 0)); // Green
		}
		else if(_color == C_BLUE || _color == C_BLUE1){
			_theaterChase(leds.Color(0, 0, 255)); // Blue
		}
		else if(_color == C_WHITE || _color == C_WHITE1){
			_theaterChase(leds.Color(127, 127, 127)); // White
		}
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


//Theatre-style crawling lights.
void RobotMotorBoard::_theaterChase(uint32_t color) {
	
	if (F_show_up){
		if (_i_pos < 3){
			if(!F_clearLEDs){
				for (int i=0; i < LED_COUNT; i=i+3) {
					if(i+_i_pos <= LED_COUNT)
						leds.setPixelColor(i+_i_pos, color);    //turn every third pixel on
				}
				leds.show();
				TimerDelay=millis();  // reset TimerDelay
				F_clearLEDs=true;
			}
			else{	// F_clearLEDs == 1
				uint32_t TimerDelay_t = millis();
				if (TimerDelay_t - TimerDelay > _wait){		// Delay for visibility
					F_StartDisplay=false;
					for (int i=0; i < LED_COUNT; i=i+3) {
						if(i+_i_pos <= LED_COUNT)
							leds.setPixelColor(i+_i_pos, 0);	//turn every third pixel off
					}
					_i_pos++;
					F_clearLEDs=false;
				}
			}
		}
		else{	// end one cycle
			if(!F_theaterChase_RGBW){
				if(F_show_Up_Down){
					F_show_up=false;
				}	
				else{ // F_show_Up_Down == 0
					if(!F_Show_cont || F_showAllStyles){
						_ncycle--;
					#ifdef DEBUG
						Serial.print(F("ncyc= "));
						Serial.println(_ncycle);
					#endif
					}
				}
			}
			else if(F_theaterChase_RGBW){ 
				if(F_show_Up_Down){
					F_show_up=false;
					_color++;
				}
				else{ // F_show_Up_Down == 0
					if(_color == 3){
						if(!F_Show_cont || F_showAllStyles){
							_ncycle--;
						#ifdef DEBUG
							Serial.print(F("ncyc= "));
							Serial.println(_ncycle);
						#endif
						}
					}
					_color++;
					_color %= 4;	//change color W,R,G,B
				}
			}
			_i_pos=0;
		}
	}
	else if(!F_show_up){	//(F_show_up == 0)
		if (_i_pos < 3){
			if(!F_clearLEDs){
				for (int i=LED_COUNT-1; i >= 0; i=i-3) {
					//if((i-_i_pos) >= 0)
						leds.setPixelColor(i-_i_pos, color);    //turn every third pixel on
				}
				leds.show();
				TimerDelay=millis();  // reset TimerDelay
				F_clearLEDs=true;
			}
			else{	// F_clearLEDs == 1
				uint32_t TimerDelay_t = millis();
				if (TimerDelay_t - TimerDelay > _wait){ // Delay for visibility
					F_StartDisplay=false;
					for (int i=LED_COUNT-1; i >= 0; i=i-3) {
						//if((i-_i_pos) >= 0)
							leds.setPixelColor(i-_i_pos, 0);        //turn every third pixel off
					}
					_i_pos++;
					F_clearLEDs=false;
				}
			}
		}
		else{	// end one cycle
			if(!F_theaterChase_RGBW){
				if(F_show_Up_Down){
					F_show_up=true;
				}	
				if(!F_Show_cont || F_showAllStyles){
					_ncycle--;
				#ifdef DEBUG
					Serial.print(F("ncyc= "));
					Serial.println(_ncycle);
				#endif
				}
			}
			else if(F_theaterChase_RGBW){ 
				if(F_show_Up_Down){
					F_show_up=true;
				}
				if(_color == 3 && !F_show_Up_Down){
					if(!F_Show_cont || F_showAllStyles){
						_ncycle--;
					#ifdef DEBUG
						Serial.print(F("ncyc= "));
						Serial.println(_ncycle);
					#endif
					}
				}
				else if(_color == 7 && F_show_Up_Down){
					if(!F_Show_cont || F_showAllStyles){
						_ncycle--;
					#ifdef DEBUG
						Serial.print(F("ncyc= "));
						Serial.println(_ncycle);
					#endif
					}
				}
				_color++;
				_color %= 8;	//change color W,R,G,B
			}
			_i_pos=0;
		}
	}
}


void RobotMotorBoard::runRainbow1(){

	if(_ncycle > 0){
		_rainbow1();
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}




void RobotMotorBoard::_rainbow1(){

	uint32_t TimerDelay_t = millis();
	if ((TimerDelay_t - TimerDelay > _wait) || F_StartDisplay){ // Delay for visibility
		F_StartDisplay = false;
		if(_i_pos < 256){
			for(uint16_t i=0; i<LED_COUNT; i++) {
				leds.setPixelColor(i, _Wheel((i+_i_pos) & 255));
			}
			leds.show();
			_i_pos++;
			TimerDelay = millis();  // reset TimerDelay
		}
		else{
			if(!F_Show_cont || F_showAllStyles)
				_ncycle--;
			_i_pos=0;
		}
	}
}



// Slightly different, this makes the rainbow equally distributed throughout
void RobotMotorBoard::runRainbowCycle(){

	if(_ncycle > 0){
		_rainbowCycle();
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::_rainbowCycle(){

	uint32_t TimerDelay_t = millis();
	if ((TimerDelay_t - TimerDelay > _wait) || F_StartDisplay){ // Delay for visibility
		F_StartDisplay = false;
		if (_i_pos < 256){	// 1 cycle of all colors on wheel
			for(byte i=0; i< LED_COUNT; i++) {
			  leds.setPixelColor(i, _Wheel(((i * 256 / LED_COUNT) + _i_pos) & 255));
			}
			leds.show();
			_i_pos++;
			TimerDelay = millis();  // reset TimerDelay
		}
		else{	// end one cycle
			if(!F_Show_cont || F_showAllStyles)
				_ncycle--;
			_i_pos=0;
		}
	}
}


//Theatre-style crawling lights with rainbow effect
void RobotMotorBoard::runTheaterChaseRainbow(){

	if(_ncycle > 0){
		_theaterChaseRainbow();
	}
	else{	// _ncycle == 0
		if(F_showAllStyles){	// get next style
			if(_checkIfAllCycEnd())
				return;
			else{
				_setForShowAll();
			}
		}
		else{
			_style=0;		// stop show leds
			F_stop=true;
		#ifdef DEBUG
			Serial.print(F("_style= "));
			Serial.println(_style);
		#endif
		}
		clearLEDs();
		leds.show();
	}
}


void RobotMotorBoard::_theaterChaseRainbow(){

	if (F_show_up){
		if(_j_pos < 256){     // cycle all 256 colors in the wheel
			if(_i_pos < 3){
				if(!F_clearLEDs){
					for (int i=0; i < LED_COUNT; i=i+3) {
						if(i+_i_pos <= LED_COUNT)
							leds.setPixelColor(i+_i_pos, _Wheel( (i+_j_pos) % 255));    //turn every third pixel on
					}
					leds.show();
					TimerDelay=millis();  // reset TimerDelay
					F_clearLEDs=true;
				}
				else{	// F_clearLEDs == 1
					uint32_t TimerDelay_t = millis();
					if (TimerDelay_t - TimerDelay > _wait){ // Delay for visibility
						F_StartDisplay=false;
						for (int i=0; i < leds.numPixels(); i=i+3) {
							if(i+_i_pos <= LED_COUNT)
								leds.setPixelColor(i+_i_pos, 0);        //turn every third pixel off
						}
						_i_pos++;
						F_clearLEDs=false;
					}
				}
			}
			else{	// end one cycle i_pos 
				_i_pos=0;
				_j_pos++;
			}
		}
		else{ // end one cycle j_pos (all 256 colors)
			if(F_show_Up_Down){
				F_show_up=false;
			}	
			else{ // F_show_Up_Down == 0
				if(!F_Show_cont || F_showAllStyles){
					_ncycle--;
				#ifdef DEBUG
					Serial.print(F("ncyc= "));
					Serial.println(_ncycle);
				#endif
				}
			}
			_j_pos=0; _i_pos=0;
		}
	}
	else if (!F_show_up){	//(F_show_up == 0)
		if(_j_pos < 256){     // cycle all 256 colors in the wheel
			if(_i_pos < 3){
				if(!F_clearLEDs){
					for (int i=LED_COUNT-1; i >= 0; i=i-3) {
						//if((i-_i_pos) >= 0)
							leds.setPixelColor(i-_i_pos, _Wheel( (i-_j_pos) % 255));    //turn every third pixel on
					}
					leds.show();
					TimerDelay=millis();  // reset TimerDelay
					F_clearLEDs=true;
				}
				else{	// F_clearLEDs == 1
					uint32_t TimerDelay_t = millis();
					if (TimerDelay_t - TimerDelay > _wait){ // Delay for visibility
						F_StartDisplay=false;
						for (int i=LED_COUNT-1; i >= 0; i=i-3){
							//if((i-_i_pos) >= 0)
								leds.setPixelColor(i-_i_pos, 0);        //turn every third pixel off
						}
						_i_pos++;
						F_clearLEDs=false;
					}
				}
			}
			else{	// end one cycle i_pos 
				_i_pos=0;
				_j_pos++;
			}
		}
		else{ // end one cycle j_pos (all 256 colors)
			if(F_show_Up_Down){
				F_show_up=true;
			}	
			if(!F_Show_cont || F_showAllStyles){
				_ncycle--;
			#ifdef DEBUG
				Serial.print(F("ncyc= "));
				Serial.println(_ncycle);
			#endif
			}
			_j_pos=0; _i_pos=0;
		}
	}
}





// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t RobotMotorBoard::_Wheel(byte WheelPos){
  if(WheelPos < 85) {
   return leds.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return leds.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return leds.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


bool RobotMotorBoard:: _checkIfAllCycEnd(){
	if(!F_Show_contAllStyles){
		if(_ncycleAllStyles == 0){
			_style=0;		// stop show leds
			F_stop=true;
			clearLEDs();
			leds.show();
		#ifdef DEBUG
			Serial.print(F("_ncycleAllStyles=_style= "));
			Serial.println(_style);
		#endif
			return true;
		}
	}
	return false;
}


void RobotMotorBoard:: _setForShowAll(){
	if(F_AllStyleRandom){
		_style=_getStyleRandom(MAX_STYLES);
		_ncycleAllStyleRandom++;
		if(!F_Show_contAllStyles){
			_ncycleAllStyles--;
		#ifdef DEBUG
			Serial.print(F("_ncycleAllStyles= "));
			Serial.println(_ncycleAllStyles);
		#endif
		}
		_initForShowAll();
	}
	else if(F_AllStyle_Up == true){
		_style++;
		if(_style >=  MAX_STYLES+1){
			_style == 1;
			if(!F_Show_contAllStyles){
				_ncycleAllStyles--;
			#ifdef DEBUG
				Serial.print(F("_ncycleAllStyles= "));
				Serial.println(_ncycleAllStyles);
			#endif
			}
		}
		_initForShowAll();
	}
	else if(F_AllStyle_Up == false){
		_style--;
		if(_style ==  0){
			_style=MAX_STYLES;
			if(!F_Show_contAllStyles){
				_ncycleAllStyles--;
			#ifdef DEBUG
				Serial.print(F("_ncycleAllStyles= "));
				Serial.println(_ncycleAllStyles);
			#endif
			}
		}
		_initForShowAll();
	}
}