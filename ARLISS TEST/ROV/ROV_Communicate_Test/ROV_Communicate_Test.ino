#include <GPS.h>
#include <Wire.h>
#include <LSM303.h>
#include <Math.h>
#include <Servo.h>
#include <SD.h>
#include<DCS.h>
#include<RF.h>

/*
Car functions
*/
void steer(float a,float b,float c,float d,float e);
void go(float a,float b,float c,float d,float e);

/*
Car Variables
*/
DCS dcs;
RF rf;
GPS gpsEx;
Servo Carsteer;
LSM303 compass;
float destlat = 37.279319;
float destlong = 126.570091;
float steer_car,dsteer_car,isteer_car = 0, steer_car0;
float Pgain = 1, Dgain, Igain;
float osteer;
int motor = 2;
int r=0,g,d;
String state = "";
String rcvPck = "";
char x [6];

const String module = "R";
const int chipSelect = 53;

void setup(){
  state = "S";
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  //for compass sensor
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    return;
  } 
  Wire.begin();
  compass.init();
  compass.enableDefault();  
  compass.m_min = (LSM303::vector<int16_t>){
    -703,-737, -746  };
  compass.m_max = (LSM303::vector<int16_t>){
    +695, +461, +635  };
  //for steer
  Carsteer.attach(3);
  //for speed
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
}

void loop(){
  String dataString = "";
  compass.read();
  float heading_ = compass.heading((LSM303::vector<int>){0,1,0});
  dtostrf(heading_,3,1,x);

  
  dcs.mergeData(module,state,"1234.5678","11234.5678","20",x);
  rf.sendPck(dcs.getRfData());
  r = rf.receivePck(rcvPck);
  if(r==0){
    dcs.readPck(rcvPck);
    Serial.print("module: ");
    Serial.println(dcs.getState());
    Serial.print("state: ");
    Serial.println(dcs.getModule());
    Serial.print("latitude: ");
    Serial.println(dcs.getSLat());
    Serial.print("longitude: ");
    Serial.println(dcs.getSLng());
    Serial.print("height: ");
    Serial.println(dcs.getSHgt());
    Serial.print("heading: ");
    Serial.println(dcs.getSHeading());
  }
  delay(1000);
}
