/*

EEPROM memory used

#define NUM_EEPROM_BMP 10

0 - 319: heder pic array [10] 12*10 =120 bytes, 

320 - 337 username, 
#define ADDRESS_USERNAME 320

338 - 355 robotname, 
#define ADDRESS_ROBOTNAME 338

356 - 373 cityname, 
#define ADDRESS_CITYNAME 356

374 - 391 countryname, 
#define ADDRESS_COUNTRYNAME 374

//392-409 robot info
#define ADDRESS_ROBOTINFO	392
#define ADDR_CHOICE_MUSIC	410


void RobotControl::getMyName(char* container){
	EEPROM_I2C::readBuffer(ADDRESS_USERNAME,(uint8_t*)container,18);
}
void RobotControl::getRobotName(char* container){
	EEPROM_I2C::readBuffer(ADDRESS_ROBOTNAME,(uint8_t*)container,18);
}
void RobotControl::getMyCity(char* container){
	EEPROM_I2C::readBuffer(ADDRESS_CITYNAME,(uint8_t*)container,18);
}
void RobotControl::getMyCountry(char* container){
	EEPROM_I2C::readBuffer(ADDRESS_COUNTRYNAME,(uint8_t*)container,18);
}

void RobotControl::setMyName(char* text){
	EEPROM_I2C::writePage(ADDRESS_USERNAME,(uint8_t*)text,18);
}
void RobotControl::setRobotName(char* text){
	EEPROM_I2C::writePage(ADDRESS_ROBOTNAME,(uint8_t*)text,18);
}
void RobotControl::setMyCity(char* text){
	EEPROM_I2C::writePage(ADDRESS_CITYNAME,(uint8_t*)text,18);
}
void RobotControl::setMyCountry(char* text){
	EEPROM_I2C::writePage(ADDRESS_COUNTRYNAME,(uint8_t*)text,18);
}


textManager mostly contains helper functions for 
R06_Wheel_Calibration and R01_Hello_User.

The ones used in this example:
  textManager.setMargin(margin_left, margin_top):
    Configure the left and top margin for text
    display. The margins will be used for 
    textManager.writeText().
    Parameters:
      margin_left, margin_top: the margin values
      from the top and left side of the screen.
    Returns:
      none
    
  textManager.writeScriptPGM(script_number,line,column):
    Display a script of Hello User example. 
    Parameters:
      script_number: an int value representing the 
        script to be displayed.
      line, column: in which line,column is the script
        displayed. Same as writeText().
    Returns:
      none
  
  textManager.inputUser(line,column,codename):
    Print an input indicator(">") in the line and column,
    dispaly and receive input from a virtual keyboard,
    and save the value into EEPROM represented by codename
    Parameters:
      line,column: int values represents where the input
        starts. Same as wirteText().
      codename: either USERNAME,ROBOTNAME,CITYNAME or
        COUNTRYNAME. You can call Robot.userNameRead(),
        robotNameRead(),cityNameRead() or countryNameRead()
        to access the values later.
    Returns:
      none;
  
  textManager.writeText(line,column,text):
    Display text on the specific line and column. 
    It's different from Robot.text() as the later
    uses pixels for positioning the text.
    Parameters:
      line:in which line is the text displayed. Each line
        is 10px high.
      column:in which column is the text displayed. Each
        column is 8px wide.
      text:a char array(string) of the text to be displayed.
    Returns:
      none
      
    textManager.showPicture(filename, x, y):
      It has the same functionality as Robot.drawPicture(),
      while fixing the conflict between drawPicture() and 
      sound playing. Using Robot.drawPicture(), it'll have
      glitches when playing sound at the same time. Using
      showPicture(), it'll stop sound when displaying 
      picture, so preventing the problem.
      Parameters:
        filename:string, name of the bmp file in sd
        x,y: int values, position of the picture
      Returns:
        none  
    

//****************************************************************
  // from now on, display different slides of 
  // text/pictures in sequence. the so-called
  // scripts are strings of text stored in the
  // robot's memory
  
  // these functions are explained below
  
  //Script 6
  textManager.writeScriptPGM((char*)scripts_User,5, 4, 0); 
  textManager.writeScriptPGM((char*)scripts_User,9, 10, 0);
  Robot.waitContinue();
  delay(500);
  Robot.clearScreen();

  //Script 7
  textManager.writeScriptPGM((char*)scripts_User,6, 4, 0);
  textManager.writeScriptPGM((char*)scripts_User,9, 10, 0);
  Robot.waitContinue();
  delay(500);
  Robot.clearScreen();

  //Script 8
  Robot._drawBMP_P(PSTR("init2.bmp"), 0, 0);
  textManager.writeScriptPGM((char*)scripts_User,7, 2, 0);
  textManager.writeScriptPGM((char*)scripts_User,9, 7, 0);
  Robot.waitContinue();
  delay(500);
  Robot.clearScreen();

  //Script 9
  Robot._drawBMP_P(PSTR("init3.bmp"), 0, 0);
  textManager.writeScriptPGM((char*)scripts_User,8, 2, 0);
  textManager.writeScriptPGM((char*)scripts_User,9, 7, 0);
  Robot.waitContinue();
  delay(500);
  Robot.clearScreen();

  //Script 11
  textManager.writeScriptPGM((char*)scripts_User,10, 4, 0);
  textManager.writeScriptPGM((char*)scripts_User,9, 10, 0);
  Robot.waitContinue();
  delay(500);
  Robot.clearScreen();

  //Input screen
  textManager.writeScriptPGM((char*)scripts_User,0, 1, 1);
  textManager.inputUser(3, 1, USERNAME);
  
  textManager.writeScriptPGM((char*)scripts_User,1, 5, 1);
  textManager.inputUser(7, 1, ROBOTNAME);

  delay(1000);
  Robot.clearScreen();
  
  //last screen
  Robot._drawBMP_P(PSTR("init4.bmp"), 0, 0);
  textManager.writeText(1, 2, "Hello");
  Robot.userNameRead(buffer);
  textManager.writeText(3, 2, buffer);

  textManager.writeScriptPGM((char*)scripts_User,4,10,0);

  Robot.waitContinue(BUTTON_LEFT);
  Robot.waitContinue(BUTTON_RIGHT);
  Robot._drawBMP_P(PSTR("kt1.bmp"), 0, 0);



 void RobotTopBoard::playSoundBuzz(uint8_t nSound){

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

//-----------------------------------------------------------------------

ADC1:****
5V = R2*UBatMAX / (R1 + R2)

UbatMAX= 5 * (R1+R2)/R2 = 5*(4,7+5,6)/5,6 = 9,2V
or
UbatMAX= 5 * (R1+R2)/R2 = 5*(10+5,6)/5,6 = 13,93V

  
ADC2:****
U2 = R2*Vdd/R1+R2 = 5,6*5/(5,6+R1);

R1=0 (Light); U2=5V

R1 => 3,4K (Dark); U2 <= 2,8V (DN = 0x023D = 573)

 DN (Digital Number) from ADC.
VBattery = VADC_IN * ((R1 + R2)/R2) = VADC_IN * 1,84
Разрешение АЦП = 5V/1024(10 digit) = 4,88 мВ/DN
VADC_IN = DN * 4,88 (мВ)
Следовательно, значение напряжения:
VBattery = 1,84 * (4,88 * DN (mV)) = 0.009 * DN (Вольт)



  
//-----------------------------------------------------------------------

void RobotMotorBoard::leds_Show(uint8_t style, uint8_t ncycleInStyle, uint8_t ncycleAllStyles, uint16_t wait)

ncycle == SHOW_CONTINUE(255) ,set flag for SHOW_CONTINUE in any style 
ncycleAllStyles == SHOW_CONTINUE(255) ,set flag for SHOW_CONTINUE for all styles

	//  set flag for SHOW_CONTINUE in style
	if(ncycleInStyle == SHOW_CONTINUE)
		F_Show_cont=true;
	else 
		F_Show_cont=false;

	//  set flag for SHOW_CONTINUE for all styles
	if(ncycleAllStyles == SHOW_CONTINUE)
		F_Show_contAllStyles=true;
	else 
		F_Show_contAllStyles=false;

	//  set flag for *_ALL_STYLES_*
	if((style == SHOW_ALL_STYLES_UP) || 
		(style == SHOW_ALL_STYLES_DOUN) ||
		(style == SHOW_ALL_STYLES_RANDOM)){

		F_showAllStyles=true;
	}
	else{
		F_showAllStyles=false;
	} 

--------------------------------------------------------------------------------------------------------------------------
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

-----------------------------------------------------------------------------------------
Robot.leds_Stop();
Robot.leds_Show(uint8_t style, uint8_t ncycleInStyle, uint8_t ncycleAllStyles, uint16_t wait);
-----------------------------------------------------------------------------------------



Joystick
---------
		 0						128							255
		
	0	128	  255		-128	 0		128			-255	  0		-255

		255						-128						-255

------------------------------------------------------------------------------------------




	structure for File.seq

	GP	nSEQ(16)	speedML(16)		speedMR(16)		Mduration(16)	Fr_Light(8)		Led_Style(8)	Led_Wait(16)

-----------------------------------------------------------------------------------------





	//--------------------------------------------------------------------
//[CHECK VOLTAGE]
//Reads the input voltage and shuts down the bot when the power drops
//--------------------------------------------------------------------
byte s_bLVBeepCnt;
boolean CheckVoltage() {

#ifdef  cVoltagePin
  #ifdef cTurnOffVol1
     Volt1 = analogRead(V_Bat1_p);                  // Battery 11.1V voltage 
     Volt1 = ((long)Volt1 * 1359) / 1000;
     Volt2 = analogRead(V_Bat2_p);                  // Battery 9v or 7V voltage 
     Volt2 = ((long)Volt2 * 1359) / 1000;
     unsigned long DisplayV1V2_t = millis();
  #ifdef DEBUG
     if ((DisplayV1V2_t - DisplayV1V2 > 30000) || StartDisplayV1V2){      // display V1, V2 every 30c
       DBGSerial.print(F("V1="));
       DBGSerial.print(Volt1, DEC);
       DBGSerial.print(F("V"));
       DBGSerial.print(F("    V2="));
       DBGSerial.print(Volt2, DEC);
       DBGSerial.println(F("V"));
       DisplayV1V2 = millis();
       StartDisplayV1V2 = false;
     }
  #endif
      if (!g_fLowVoltageShutdown) { // == false
        if ((Volt1 < cTurnOffVol1) || (Volt1 > 1260)) {
       #ifdef DBGSerial          
          DBGSerial.print(F("Voltage V1 error, Turn off robot! V1="));
          DBGSerial.print(Volt1, DEC);
          DBGSerial.println(F("V"));
       #endif            
          if (CountVolt1Shutdown > MAX_Count_Voltage) {
            //Turn off
            g_InControlState.BodyPos.x = 0;
            g_InControlState.BodyPos.y = 0;
            g_InControlState.BodyPos.z = 0;
            g_InControlState.BodyRot1.x = 0;
            g_InControlState.BodyRot1.y = 0;
            g_InControlState.BodyRot1.z = 0;
            g_InControlState.TravelLength.x = 0;
            g_InControlState.TravelLength.z = 0;
            g_InControlState.TravelLength.y = 0;
            g_BodyYOffset = 0;
            g_BodyYShift = 0;
            g_InControlState.SelectedLeg = 255;
            g_fLowVoltageShutdown = true;
            s_bLVBeepCnt = 0;                            // reset Beep counter
            g_InControlState.fHexOn = false;
            CountVolt1Shutdown = 0;                      // reset counter
            CountVolt2Shutdown = 0;
            f_LowVolt1Shutdown = true;
         #ifdef DBGSerial          
            DBGSerial.println(F("Shutdown V1!!!"));
         #endif            
          }  
          else {
            CountVolt1Shutdown++;
         #ifdef DBGSerial          
            DBGSerial.print(F("CountVolt1Shutdown="));
            DBGSerial.println(CountVolt1Shutdown, DEC);
         #endif            
          }
          if ((Volt1 >= cTurnOffVol1) && (Volt1 <= 1260)) {
            CountVolt1Shutdown=0;
          }  
        }
        if ((Volt2 < cTurnOffVol2) || (Volt2 > 920)) {
       #ifdef DBGSerial          
          DBGSerial.print(F("Voltage V2 error, Turn off robot! V2="));
          DBGSerial.print(Volt2, DEC);
          DBGSerial.println(F("V"));
       #endif            
          if (CountVolt2Shutdown > MAX_Count_Voltage) {
            //Turn off
            g_InControlState.BodyPos.x = 0;
            g_InControlState.BodyPos.y = 0;
            g_InControlState.BodyPos.z = 0;
            g_InControlState.BodyRot1.x = 0;
            g_InControlState.BodyRot1.y = 0;
            g_InControlState.BodyRot1.z = 0;
            g_InControlState.TravelLength.x = 0;
            g_InControlState.TravelLength.z = 0;
            g_InControlState.TravelLength.y = 0;
            g_BodyYOffset = 0;
            g_BodyYShift = 0;
            g_InControlState.SelectedLeg = 255;
            g_fLowVoltageShutdown = true;
            s_bLVBeepCnt = 0;                            // reset Beep counter 
            g_InControlState.fHexOn = false;
            CountVolt1Shutdown = 0;                      // reset counter
            CountVolt2Shutdown = 0;
            f_LowVolt2Shutdown = true;
         #ifdef DBGSerial          
            DBGSerial.println(F("Shutdown V2!!!"));
         #endif            
          }  
          else {
            CountVolt2Shutdown++;
         #ifdef DBGSerial          
            DBGSerial.print(F("CountVolt2Shutdown="));
            DBGSerial.println(CountVolt2Shutdown, DEC);
         #endif            
          }
        }  
        if ((Volt2 >= cTurnOffVol2) && (Volt2 <= 920)) {
          CountVolt2Shutdown=0;
        }  
     #ifdef cTurnOnVol1
      } // g_fLowVoltageShutdown == true
      else {
        if (((Volt1 > cTurnOnVol1) && (Volt1 < 1260))  && !f_LowVolt2Shutdown && f_LowVolt1Shutdown) {
        #ifdef DBGSerial
          DBGSerial.print(F("Voltage V1 restored! V1="));
          DBGSerial.print(Volt1, DEC);
          DBGSerial.println(F("V"));
        #endif          
        g_fLowVoltageShutdown = false;
        f_LowVolt1Shutdown = false;
        CountVolt1Shutdown=0;
        }
        if ((Volt1 > cTurnOnVol1) && (Volt1 < 1260)){
          f_LowVolt1Shutdown = false;
        }  
        else {
          if (s_bLVBeepCnt < 5) {
            s_bLVBeepCnt++;
            MSound( 1, 45, 2000);                // [45\4000] 
          }
          delay(2000);
        }
        if (((Volt2 > cTurnOnVol2) && (Volt2 < 920)) && !f_LowVolt1Shutdown && f_LowVolt2Shutdown) {
        #ifdef DBGSerial
          DBGSerial.print(F("Voltage V2 restored! V2="));
          DBGSerial.print(Volt2, DEC);
          DBGSerial.println(F("V"));
        #endif          
        g_fLowVoltageShutdown = false;
        f_LowVolt2Shutdown = false;
        CountVolt2Shutdown=0;
        }
        if ((Volt2 > cTurnOnVol2) && (Volt2 < 1260)){
          f_LowVolt2Shutdown = false;
        }  
        else {
          if (s_bLVBeepCnt < 5) {
            s_bLVBeepCnt++;
            MSound( 1, 45, 2000);                // [45\4000] 
          }
          delay(2000);
        }
     #endif      //cTurnOnVol1
      } 
  #endif  // cTurnOffVol1
#endif    //cVoltagePin
  return g_fLowVoltageShutdown;
} 


*** Cooqrobot_ISD_V9_R1.ino
------------------------------

    Serial.print("!3N");
    answ = GetByte()<<8;
    answ += GetByte();
    v=(float)(answ * 0.0095);
    return v;


	//----------------------------------------
  void Print_V_Bat(void){
    float v;
    
    v=Get_V_Bat();
    lcd.setCursor(0,0);    
    lcd.print("P. Bat = ");
    lcd.print(v);
    lcd.print(" V");
  }






R1 = 10k
R2 = 5.6k
-----------------
R1 = 4.7k
R2 = 5.6k










*/