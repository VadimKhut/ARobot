

#include <avr/pgmspace.h>

//an advanced trick for storing strings inside the program space
//as the ram of Arduino is very tiny, keeping too many string in it
//can kill the program

const char hello_user_script1[] PROGMEM = {"What's your name?"};
const char hello_user_script2[] PROGMEM = {"Give me a name!"};
const char hello_user_script3[] PROGMEM = {"And the country?"};
const char hello_user_script4[] PROGMEM = {"The city you're in?"};
const char hello_user_script5[] PROGMEM = {"  Plug me to\n\n  your computer\n\n  and start coding!"};

const char hello_user_script6[] PROGMEM = {"     Hello User!\n\n It's me, your robot\n\n    I'm alive! <3"};
const char hello_user_script7[] PROGMEM = {"  First I need some\n\n   input from you!"};
const char hello_user_script8[] PROGMEM = {"     Use the knob\n\n   to select letters"};
const char hello_user_script9[] PROGMEM = {"   Use L/R button\n\n to move the cursor,\n\n  middle to confirm"};
const char hello_user_script10[] PROGMEM = {"  Press middle key\n   to continue..."};
const char hello_user_script11[] PROGMEM = {"  Choose \"enter\" to\n\n  finish the input"};
const char hello_user_script12[] PROGMEM = {"M E N U"};
const char hello_user_script13[] PROGMEM = {"Pleased to meet you!\n\n   Come and play\n    with Me!"};


const char main_menu_script1[] PROGMEM = {" 1. Run"};
const char main_menu_script2[] PROGMEM = {" 2. Disco Bot"};
const char main_menu_script3[] PROGMEM = {" 3. Picture browser"};
const char main_menu_script4[] PROGMEM = {" 4. Line follow"};
const char main_menu_script5[] PROGMEM = {" 5. Rescue bot"};
const char main_menu_script6[] PROGMEM = {" 6. Run pattern"};
const char main_menu_script7[] PROGMEM = {" 7. Service"};


//const char dance_menu_script1[] PROGMEM = {" 1. Macarena"};
//const char dance_menu_script2[] PROGMEM = {" 2. Lambada"};
//const char dance_menu_script3[] PROGMEM = {" 3. Kiss Kiss"};
//const char dance_menu_script4[] PROGMEM = {" 4. Americana"};
//const char dance_menu_script5[] PROGMEM = {" 5. Empty"};
//const char dance_menu_script6[] PROGMEM = {" 6. Empty"};
//const char dance_menu_script7[] PROGMEM = {" 7. Exit"};


const char choiceMusic_menu_scr1[] PROGMEM = {" 1. Macarena"};
const char choiceMusic_menu_scr2[] PROGMEM = {" 2. Lambada"};
const char choiceMusic_menu_scr3[] PROGMEM = {" 3. Kiss Kiss"};
const char choiceMusic_menu_scr4[] PROGMEM = {" 4. Americana"};
const char choiceMusic_menu_scr5[] PROGMEM = {" 5. Empty"};
const char choiceMusic_menu_scr6[] PROGMEM = {" 6. Empty"};
const char choiceMusic_menu_scr7[] PROGMEM = {" 7. Exit"};


const char faleName_scr1[] PROGMEM = {"Macarena"};
const char faleName_scr2[] PROGMEM = {"Lambada"};
const char faleName_scr3[] PROGMEM = {"Muzyka1"};
const char faleName_scr4[] PROGMEM = {"America"};
const char faleName_scr5[] PROGMEM = {"Belly2"};
const char faleName_scr6[] PROGMEM = {"Belly3"};
//const char faleName_scr7[] PROGMEM = {"Belly4"};
//const char faleName_scr8[] PROGMEM = {"Belly5"};
//const char faleName_scr9[] PROGMEM = {"belly111"};


const char serv_menu_script1[] PROGMEM = {" 1. Set User Data"};
const char serv_menu_script2[] PROGMEM = {" 2. Get Info"};
const char serv_menu_script3[] PROGMEM = {" 3. Set Time"};
const char serv_menu_script4[] PROGMEM = {" 4. Wheel calibrat."};
const char serv_menu_script5[] PROGMEM = {" 5. Choice music"};
const char serv_menu_script6[] PROGMEM = {" 6. Max pict. coll."};
const char serv_menu_script7[] PROGMEM = {" 7. Exit"};


const char mode_name_script1[] PROGMEM = {"keyboard"};
const char mode_name_script2[] PROGMEM = {"tilt"};

const char FaleExt_wav_scr[] PROGMEM = {".wav"};
const char FaleExt_gps_scr[] PROGMEM = {".gps"};









const PROGMEM char* const PROGMEM scripts_User[] = {
  hello_user_script1,
  hello_user_script2,
  hello_user_script3,
  hello_user_script4,
  hello_user_script5,
  hello_user_script6,
  hello_user_script7,
  hello_user_script8,
  hello_user_script9,
  hello_user_script10,
  hello_user_script11,
  hello_user_script12,
  hello_user_script13
};


const PROGMEM char* const PROGMEM scripts_Main_Menu[] = {
  main_menu_script1,
  main_menu_script2,
  main_menu_script3,
  main_menu_script4,
  main_menu_script5,
  main_menu_script6,
  main_menu_script7
};


//const PROGMEM char* const PROGMEM scripts_Dance_Menu[] = {
//  dance_menu_script1,
//  dance_menu_script2,
//  dance_menu_script3,
//  dance_menu_script4,
//  dance_menu_script5,
//  dance_menu_script6,
//  dance_menu_script7
//};


const PROGMEM char* const PROGMEM scripts_Serv_Menu[] = {
  serv_menu_script1,
  serv_menu_script2,
  serv_menu_script3,
  serv_menu_script4,
  serv_menu_script5,
  serv_menu_script6,
  serv_menu_script7
};


const PROGMEM char* const PROGMEM scripts_Choice_Music[] = {
  choiceMusic_menu_scr1,
  choiceMusic_menu_scr2,
  choiceMusic_menu_scr3,
  choiceMusic_menu_scr4,
  choiceMusic_menu_scr5,
  choiceMusic_menu_scr6,
  choiceMusic_menu_scr7
};


const PROGMEM char* const PROGMEM scripts_FaleNameMusic[] = {
  faleName_scr1,
  faleName_scr2,
  faleName_scr3,
  faleName_scr4,
  faleName_scr5,
  faleName_scr6
};


const PROGMEM char* const PROGMEM scripts_ModeName[] = {
  mode_name_script1,
  mode_name_script2
};








/*
void getPGMtext(int seq){
  //It takes a string from program space, and fill it  
  //in the buffer
  strcpy_P(buffer,(char*)pgm_read_word(&(scripts[seq])));
}

void writeScript(int seq, int line, int col){
  //print a string from program space to a specific line,
  //column on the LCD
  
  //first fill the buffer with text from program space
  getPGMtext(seq);
  //then print it to the screen
  textManager.writeText(line,col,buffer);
}

*/