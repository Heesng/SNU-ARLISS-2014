//Tranmission Example for RF module.
//RF module uses Seraial1 in this library.

#include <RF.h>

RF rfEx;

void setup(){
  Serial.begin(9600);
  Serial1.begin(1200);
}

void loop(){
  String A = "";
  while(Serial.available()>0){
    char c = Serial.read();
    A += c
  }
  if(A != ""){
    rfEx.SendPacket(A);
  }
  delay(500);
}
