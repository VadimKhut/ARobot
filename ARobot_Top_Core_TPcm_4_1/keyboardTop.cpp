#include "ArduinoRobotTopBoard.h"
#include "Arduino.h"

int vals_pul[] = { 0, 133, 305, 481, 724 };
int pul_min[]=   { 0, 133, 319, 494, 732 };
int pul_max[]=   {10, 153, 339, 514, 752 };
/*int pul_min[]={0,123,295,471,714};
int pul_max[]={0,143,315,491,734};*/
/*
int pul_min[]={0,133,319,494,732};
int pul_max[]={10,153,339,514,752};
*/
void sort(int* v);
int _averageAnalogInput(void);



void RobotTopBoard::_keybordCalibrate(){

	for(int i=0;i<5;i++){
		vals_pul[i]=messageIn.readInt();
		pul_min[i]=vals_pul[i]-10;
		pul_min[i]=vals_pul[i]+10; 
	}
}

int8_t RobotTopBoard::_averageKeyboardRead(void){

  int lectura_pul;
  int8_t conta_pul=0;
  static int anterior=0;

  lectura_pul=::_averageAnalogInput();

  while ((conta_pul < NUMBER_BUTTONS) && !(lectura_pul >= pul_min[conta_pul] && lectura_pul <= pul_max[conta_pul]))
    conta_pul++;

  if (conta_pul >= NUMBER_BUTTONS)
    conta_pul = -1;
  else
    delay(100);

  return conta_pul;
}

int _averageAnalogInput(void){
  int vals[5];
  for(int i=0;i<5;i++){
	for(int j=i;j<5;j++){
	  vals[j]=::analogRead(KEY);
	}
	sort(vals);
  }
  return vals[0];
}

void sort(int* v){
  int tmp;
  for(int i=0;i<4;i++)
    for(int j=i+1;j<5;j++)
      if(v[j]<v[i]){
        tmp=v[j];
        v[j]=v[i];
        v[i]=tmp;
      }
  v[0]=v[3];
}