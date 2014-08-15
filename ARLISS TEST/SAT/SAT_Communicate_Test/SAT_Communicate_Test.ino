#include <ADCS.h>
#include <Compass.h>
#include <GPS.h>
#include <RF.h>
#include <SD.h>
//#include <DCS.h>
#include <math.h> 
#include <Wire.h>

ADCS adcs;
Compass comp;
GPS gps;
RF rf;

const String module = "S";
const int ledPin = 22;

void mergeData(String module_, String state_, String slat_, String slng_, String shgt_, String sheading_);
int readPck(String pck_);

int a,d,g=0, r=0; //adcs,dcs,gps,rfrcv

String state = "";
String rcvPck = "";


String rfData = "";
String sdData = "";

String rmodule;//Received module
String rstate;//Received state
String rslat;//Received String latitude
String rslng;//Received String longitude
String rshgt;//Received String height
String rsheading;//Received String latitude

float rlat = 0;
float rlng = 0;
float rhgt = 0;
float rheading = 0;


void setup(){
	state = "S";
	Serial.begin(9600);
	Serial1.begin(1200);
	Serial2.begin(9600);
	Wire.begin();
}

void loop(){

	comp.renew();
	mergeData(module,state,"1234.5678","11234.5678","20",comp.getSHeading());
	rf.sendPck(rfData);
	r = rf.receivePck(rcvPck);

	if(r==0){
		readPck(rcvPck);
		Serial.print("module: ");
		Serial.println(rmodule);
		Serial.print("state: ");
		Serial.println(rstate);
		Serial.print("latitude: ");
		Serial.println(rslat);
		Serial.print("longitude: ");
		Serial.println(rslng);
		Serial.print("height: ");
		Serial.println(rshgt);
		Serial.print("heading: ");
		Serial.println(rsheading);
	}
}


void mergeData(String module_, String state_, String slat_, String slng_, String shgt_, String sheading_){

  rfData = module_ +"," +state_ +"," + slat_ +"," + slng_ +"," + shgt_ +"," + sheading_;
  sdData = module_ +"\t"+state_ +"\t"+ slat_ +"\t"+ slng_ +"\t"+ shgt_ +"\t"+ sheading_;
}

int readPck(String pck_){

  char comma = ',';

  int i = pck_.indexOf(comma);
  rmodule = pck_.substring(0,i-1);

  int j = pck_.indexOf(comma,i+1);
  rstate = pck_.substring(i+1,j-1);

  i = pck_.indexOf(comma,j+1);
  rslat = pck_.substring(j+1,i-1);

  j= pck_.indexOf(comma,i+1);
  rslng = pck_.substring(i+1,j-1);

  i = pck_.indexOf(comma,j+1);
  rshgt = pck_.substring(j+1,i-1);

  j= pck_.indexOf(comma,i+1);
  rsheading = pck_.substring(i+1,j-1);

  char buf0[rslat.length()];
  char buf1[rslng.length()];
  char buf2[rshgt.length()];
  char buf3[rsheading.length()];

  if(rslat.length() != 0)
  {
  	rslat.toCharArray(buf0,rslat.length());
  	rlat = atof(buf0); 
    rslng.toCharArray(buf1,rslng.length());
    rlng = atof(buf1); 
    rshgt.toCharArray(buf2,rshgt.length());
    rhgt = atof(buf2);
    rsheading.toCharArray(buf2,rsheading.length());
    rheading = atof(buf3);

    return 0;
  }

  else{
    rlat = 0;
    rlng = 0;
    rhgt = 0;
    return -1;
  }
}
