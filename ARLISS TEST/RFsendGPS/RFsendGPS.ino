#include <GPS.h>
#include <DCS.h>
#include <RF.h>

RF rfTest;
GPS gpsTest;
DCS dcsTest;
String A = "HIHIHIHIHIHIHI";
int i;

void setup(){
    Serial.begin(9600);
    Serial1.begin(1200);
    Serial2.begin(9600);
}

void loop(){
	/*
	i = gpsTest.renewGPS();
	if(i == 0){
	  dcsTest.makePck(gpsTest.getSLat(),gpsTest.getSLng(),gpsTest.getSHgt());
	  A = dcsTest.getPck();
	  rfTest.SendPacket(A);
    	  Serial.println(A);
    	  delay(1000);
	}*/
	rfTest.SendPacket(A);
	delay(1000);

}

