#include <RF.h>
#include <DCS.h>
RF rfTest;
DCS dcsTest;

void setup(){
  Serial.begin(9600);
  Serial1.begin(1200);
  Serial.print("Let's begin");
}

void loop(){
  /*String A = "";
  char c;
  while(Serial1.available())
  {
    c = Serial1.read();
    A += c;
  }
  if( A!="")
  {
    Serial.println(A);
  }*/
  
  /*
  String A = "";
  A = rfTest.ReceivePacket();
      Serial.println(A);
  delay(1000);
  int i = dcsTest.readPck(A);
  if(i == 0){
   Serial.println(dcsTest.getPck());
   Serial.print(dcsTest.getLat(),4);
   Serial.print(dcsTest.getLng(),4);
   Serial.println(dcsTest.getHgt(),4);

  }*/

  String A = "";
  A = rfTest.ReceivePacket();
  Serial.println(A);
}