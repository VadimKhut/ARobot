#include "EasyTransfer2.h"




//Captures address and size of struct
void EasyTransfer2::begin(HardwareSerial *theSerial){
	_serial = theSerial;
	
	//dynamic creation of rx parsing buffer in RAM
	//rx_buffer = (uint8_t*) malloc(size);
	
	resetData();
}

void EasyTransfer2::writeByte(uint8_t dat){
		data[position++]=dat;
		size++;
}
void EasyTransfer2::writeInt(int dat){
		data[position++]=dat>>8;
		data[position++]=dat;
		size+=2;
}

void EasyTransfer2::writeStr_P(char *str_P){
	char str_t[13];
	uint8_t len;

	strcpy_P(str_t, str_P);
	len = strlen(str_t);

	Serial.print(str_t);
	
	for(uint8_t i=0; i<len; i++){
		data[position++]=str_t[i];
	}
	size+=len;
}

void EasyTransfer2::writeStr(char *str){
	char str_t[13];
	uint8_t len;

	strcpy(str_t, str);
	len = strlen(str_t);
	
	Serial.print(str_t);

	for(uint8_t i=0; i<len; i++){
		data[position++]=str_t[i];
	}
	size+=len;
}

uint8_t EasyTransfer2::readByte(){
	return data[position++];
}
int EasyTransfer2::readInt(){
	int dat_1=data[position++]<<8;
	int dat_2=data[position++];
	int dat= dat_1 | dat_2;
	return dat;
}

void EasyTransfer2::readStr(char* Str, byte shift){
	uint8_t j=0;

	for(uint8_t i=0; i<(size-shift)-1; i++){
		Str[i]=data[position++];
		j++;
	}
	Str[j]='\0';  // put end string
}


void EasyTransfer2::resetData(){
	for(int i=0;i<20;i++){
		data[i]=0;
	}
	size=0;
	position=0;
}

//Sends out struct in binary, with header, length info and checksum
void EasyTransfer2::sendData(){
  uint8_t CS = size;
  _serial->write(0x06);
  _serial->write(0x85);
  _serial->write(size);
  for(int i = 0; i<size; i++){
    CS^=*(data+i);
    _serial->write(*(data+i));
	//Serial.print(*(data+i));
	//Serial.print(",");
  }
  //Serial.println("");
  _serial->write(CS);
  
  resetData();
}

boolean EasyTransfer2::receiveData(){
  
  //start off by looking for the header bytes. If they were already found in a previous call, skip it.
  if(rx_len == 0){
  //this size check may be redundant due to the size check below, but for now I'll leave it the way it is.
    if(_serial->available() >= 3){
	//this will block until a 0x06 is found or buffer size becomes less then 3.
      while(_serial->read() != 0x06) {
		//This will trash any preamble junk in the serial buffer
		//but we need to make sure there is enough in the buffer to process while we trash the rest
		//if the buffer becomes too empty, we will escape and try again on the next call
		if(_serial->available() < 3)
			return false;
		}
		//Serial.println("head");
      if (_serial->read() == 0x85){
        rx_len = _serial->read();
		//Serial.print("rx_len:");
		//Serial.println(rx_len);
		resetData();

		//make sure the binary structs on both Arduinos are the same size.
        /*if(rx_len != size){
          rx_len = 0;
          return false;
        }*/
      }
    }
	//Serial.println("nothing");
  }
  
  //we get here if we already found the header bytes, the struct size matched what we know, and now we are byte aligned.
  if(rx_len != 0){
	
    while(_serial->available() && rx_array_inx <= rx_len){
      data[rx_array_inx++] = _serial->read();
    }
    
    if(rx_len == (rx_array_inx-1)){
      //seem to have got whole message
      //last uint8_t is CS
      calc_CS = rx_len;
	  //Serial.print("len:");
	  //Serial.println(rx_len);
      for (int i = 0; i<rx_len; i++){
        calc_CS^=data[i];
		//Serial.print("m");
		//Serial.print(data[i]);
		//Serial.print(",");
      } 
	  //Serial.println();
      //Serial.print(data[rx_array_inx-1]);
	  //Serial.print(" ");
	  //Serial.println(calc_CS);
	  
      if(calc_CS == data[rx_array_inx-1]){//CS good
		//resetData();
        //memcpy(data,d,rx_len);
		for(int i=0;i<20;i++){
			//Serial.print(data[i]);
			//Serial.print(",");
		}
		//Serial.println("");
		size=rx_len;
		rx_len = 0;
		rx_array_inx = 0;
		return true;
	  }
	  else{
  		//Serial.println("CS");
		resetData();
	  //failed checksum, need to clear this out anyway
		rx_len = 0;
		rx_array_inx = 0;
		return false;
	  }
        
    }
  }
  //Serial.print(rx_len);
  //Serial.print(" ");
  //Serial.print(rx_array_inx);
  //Serial.print(" ");
  //Serial.println("Short");
  return false;
}
