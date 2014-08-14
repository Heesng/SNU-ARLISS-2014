#include <GPS.h>
#include <RF.h>
#include <SD.h>
#include <DCS.h>
#include <math.h>
#include <LSM303.h>
#include <Wire.h>


DCS dcs;
GPS gps;
RF rf;
LSM303 compass;


const String module = "R";
const int chipSelect = 9; //SD card chip select

void sdWrite(String dataString);
float distance(GPS gps_, DCS dcs_);

int d,g=0, r=0; //dcs,gps,rfrcv
int D=0; //, Docking
float dist=1000;
String state = "";
String rcvPck = "";
char Buffer[5];
float heading_;
String sHeading;


void setup(){
      compass.init();
    compass.enableDefault();  
    compass.m_min = (LSM303::vector<int16_t>){
    -703,-737, -746  };
  compass.m_max = (LSM303::vector<int16_t>){
    +695, +461, +635  };

  
	state = "S";
	Serial.begin(9600);
	Serial1.begin(1200);
	Serial2.begin(9600);
}

void loop(){


	g = gps.renew();
        compass.read();

        heading_ = compass.heading();
        sHeading = dtostrf(heading_, 1, 4, Buffer);
	if(g==1){
		dcs.mergeData(module,state,gps.getSLat(),gps.getSLng(),gps.getSHgt(),sHeading);
		Serial.println("Data from ROV");
		Serial.println(dcs.getSdData());
		Serial.println(dcs.getRfData());
	}
	r = rf.receivePck(rcvPck);
	if(r==0){
		Serial.println(rcvPck);
		dcs.readPck(rcvPck);
		dist = distance(gps,dcs);
		Serial.print("Distance: ");
		Serial.println(dist);
	}

/*
use dcs functions dcs.function();

 	String getSLat();
	String getSLng();
	String getSHgt();
	String getSHeading();
	double getLat();
	double getLng();
	double getHgt();
	double getHeading();

*/

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
