#include "Arduino.h"
#include <Wire.h>


#ifndef Compass_h
#define Compass_h

class Compass{
public:
	Compass();
	void renew();
	float getHeading(){return heading;}
  String getSHeading(){return sHeading;}
private:
	int HMC6352SlaveAddress;
	int HMC6352ReadAddress; //"A" in hex, A command is:
	float heading;
  String sHeading;
  char Buffer[5];
};

Compass::Compass(){

	HMC6352SlaveAddress = 0x42;
	HMC6352ReadAddress = 0x41; //"A" in hex, A command is: 

	HMC6352SlaveAddress = HMC6352SlaveAddress >> 1;
}

void Compass::renew(){

  Wire.beginTransmission(HMC6352SlaveAddress);
  Wire.write(HMC6352ReadAddress);              // The "Get Data" command
  Wire.endTransmission();
  //time delays required by HMC6352 upon receipt of the command
  //Get Data. Compensate and Calculate New Heading : 6ms
  delay(6);

  Wire.requestFrom(HMC6352SlaveAddress, 2); //get the two data bytes, MSB and LSB

  //"The heading output data will be the value in tenths of degrees
  //from zero to 3599 and provided in binary format over the two bytes."
  byte MSB = Wire.read();
  byte LSB = Wire.read();

  heading = ((MSB << 8) + LSB) / 10;
  sHeading = dtostrf(heading, 1, 4, Buffer);
}

#endif