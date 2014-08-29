#include<Servo.h>
#include<CTS.h>
#include<HerkuleX.h>
#include<GPS.h>

#define RX 0
#define TX 1
#define MOTORID 219

void insert(float a);

CTS CTS1;
GPS gpsEx;
Servo Carsteer;
Servo Camera;
float rsangle = 0;
int motor = 2;
int val = 0;
int data=0;
int center=120;
int sonar1 = 49;
int sonardist1 = 100;
int offset = 11;

void setup(){ 
  delay(3000);
  //Serial.begin(9600);
  HerkuleX.begin(57600, RX, TX);  
  delay(10);
  // Torque ON
  HerkuleX.torqueOn(MOTORID);
  if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
    HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
  }
  HerkuleX.moveAngle(MOTORID, -30, 100, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);
  delay(1000);
  
  Serial3.begin(115200);
  Carsteer.attach(3);
  Camera.attach(4);
  pinMode(2,OUTPUT);
  delay(1000);
  for(int i = 56; i <= 104 ; i++){
    analogWrite(motor, i);
    delay(10);
  }
  analogWrite(motor, 170);
  delay(1000);
  analogWrite(motor, 190);
  delay(1000);
  analogWrite(motor, 181);
  delay(1000);
  
  pinMode(sonar1, INPUT);
}

void loop(){
  int i = 0;
  Camera.write(90);
  delay(100);
  sonardist1 = gpsEx.liftsonar(sonar1);
  if(sonardist1 == 200){
    for(i = 0;i<180;i++){
      Camera.write(i);
      sonardist1 = gpsEx.liftsonar(sonar1);
      delay(100); 
      if(sonardist1 != 200){
        break;
      }
    }
    if(0<=i&&i<90){
      Carsteer.write(110-offset);
      delay(100); 
      analogWrite(motor, 175);    
      delay(300);
      Carsteer.write(70-offset);
      delay(100);
      analogWrite(motor, 175);
      delay(300);
      Carsteer.write(90-offset);
      delay(100);
    }
    else if(90<=i&&i<180){
      Carsteer.write(70-offset);
      delay(100); 
      analogWrite(motor, 175);    
      delay(300);
      Carsteer.write(110-offset);
      delay(100);
      analogWrite(motor, 175);
      delay(300);
      Carsteer.write(90-offset);
      delay(100);
    }
    else{
      Carsteer.write(90-offset);
      delay(100);
      analogWrite(motor, 186);
      delay(100);
    }    
  }
  else{
    if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
      HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
    }
    HerkuleX.moveAngle(MOTORID, 150, 100, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);
    delay(1000);
    while(sonardist1>5){
      sonardist1 = gpsEx.liftsonar(sonar1);
      val = CTS1.location();
      insert(val);
    }
    if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
      HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
    }
    HerkuleX.moveAngle(MOTORID, -30, 100, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);
    delay(1000);
    sonardist1 = 100;
  }
}

void insert(float rsangle){
  if(-40<rsangle&&rsangle<40){
    Carsteer.write(90-offset);
    delay(100);
    analogWrite(motor, 186);
    delay(100);
  }
  else if(40<rsangle&&rsangle<120){
    Carsteer.write(70-offset);
    delay(100);
    analogWrite(motor, 175);
    delay(300);
    Carsteer.write(110-offset);
    delay(100); 
    analogWrite(motor, 175);
    delay(300);
    Carsteer.write(90-offset);
    delay(100);
    analogWrite(motor, 186);
    delay(400);
  }
  else if(-120<rsangle&&rsangle<-40){
    Carsteer.write(110-offset);
    delay(100); 
    analogWrite(motor, 175);    
    delay(300);
    Carsteer.write(70-offset);
    delay(100);
    analogWrite(motor, 175);
    delay(300);
    Carsteer.write(90-offset);
    delay(100);
    analogWrite(motor, 186);
    delay(400);    
  }
  else if (rsangle == 200){
    Carsteer.write(90-offset);
    delay(100);
    analogWrite(motor, 181);
    delay(100);
  }
}
