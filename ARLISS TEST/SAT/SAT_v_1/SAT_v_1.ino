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

String module = "S";
int chipSelect = 8;

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
	pinMode(10, OUTPUT);

	while(g==0){
		g = gps.renew();
	}
}

void loop(){

	a = adcs.renew();
	comp.renew();

	g = gps.renew();
	if(g==1){
		dcs.mergeData(module,state,gps.getSLat(),gps.getSLng(),gps.getSHgt(),comp.getSHeading());
		sdWrite(dcs.getSdData());
	}

	dHgt = gps.getDeltaH();


	//////////////////////Falling mode/////////////////
	if(dHgt>3)
	{
		state = "F";
		if(g==1){
			rf.sendPck(dcs.getRfData());
		}
	}

	//////////////////////Landing mode/////////////////
	else if(dist>3)
	{
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

	//////////////////////Docking mode/////////////////
	else
	{
		state = "D";
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
}

void sdWrite(String dataString){
	File dataFile = SD.open("datalog.txt", FILE_WRITE);

 	 // if the file is available, write to it:
 	 if(dataFile){
 	 	dataFile.println(dataString);
 	 	dataFile.close();

    	// print to the serial port too:
    	Serial.println(dataString);
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