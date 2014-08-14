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
const int chipSelect = 9;
const int ledPin = 22;

void sdWrite(String dataString);
float distance(GPS gps_, DCS dcs_);

int a,d,g=0, r=0; //adcs,dcs,gps,rfrcv
int P=0, D=0; //Parachute, Docking
float dHgt=0, dist=1000;
String state = "";
String rcvPck = "";

void setup(){
	state = "S";
	Serial.begin(9600);
	Serial1.begin(1200);
	Serial2.begin(9600);
	Wire.begin();
	pinMode(ledPin, OUTPUT);


	digitalWrite(ledPin, HIGH);
	delay(10000);
}

void loop(){

	a = adcs.renew();
	comp.renew();

	g = gps.renew();
	if(g==1){
		dcs.mergeData(module,state,gps.getSLat(),gps.getSLng(),gps.getSHgt(),comp.getSHeading());
	}

	dHgt = gps.getDeltaH();

	
		state = "L";
		if(P==0){	//conduct only once!!
			adcs.reelPara();
			P=1;
		}
		if(a==1) adcs.control();
		if(g==1){
			rf.sendPck(dcs.getRfData());
		}
		r = rf.receivePck(rcvPck);
		if(r==0){
			dcs.readPck(rcvPck);
			dist = distance(gps,dcs);
		}
	
}


float distance(GPS gps_, DCS dcs_){
	float lat1 = gps_.getLat();
	float lng1 = gps_.getLng();

	float lat2 = dcs_.getLat();
	float lng2 = dcs_.getLng();

	float latL, lngL, dist;
	latL = 2*3.14159*6378000*(lat1-lat2)/100/180;
	lngL = 2*3.14159*6378000*(lat1-lat2)/100/180;

	dist = sqrt(latL*latL+lngL*lngL);

	return dist;
}
