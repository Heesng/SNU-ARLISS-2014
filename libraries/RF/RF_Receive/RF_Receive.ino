//Receive Example for RF module.
//RF module uses Seraial1 in this library.

#include <RF.h>

RF rfEx;

void setup(){
  Serial.begin(9600);
  Serial1.begin(1200);
}

void loop(){
  String A = "";

  A = rfEx.ReceivePacket();
  if(A != ""){
    Serial.println(A);
  } 
}
