#include <ADCS.h>
#include <Compass.h>
#include <GPS.h>
#include <RF.h>
#include <SD.h>
#include <DCS.h>
#include <math.h> 
#include <Wire.h>

ADCS adcs;
Compass comp;
DCS dcs;
GPS gps;
RF rf;

const String module = "S";
const int ledPin = 22;

void sdWrite(String dataString);
float distance(GPS gps_, DCS dcs_);

int a,d,g=0, r=0; //adcs,dcs,gps,rfrcv
String state = "";
String rcvPck = "";

void setup(){
	state = "S";
	Serial.begin(9600);
	Serial1.begin(1200);
	Serial2.begin(9600);
	Wire.begin();
}

void loop(){

	comp.renew();
	dcs.mergeData(module,state,"1234.5678","11234.5678","20",comp.getSHeading());
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
}
