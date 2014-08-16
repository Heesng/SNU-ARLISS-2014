#include "Arduino.h"
#include <Wire.h>
#include "Compass.h"

Compass::Compass(){

	SlaveAddress = 0x42;
	ReadAddress = 0x41; //"A" in hex, A command is: 

	SlaveAddress = SlaveAddress >> 1;
}

void Compass::renew(){

  Wire.beginTransmission(SlaveAddress);
  Wire.write(ReadAddress);              // The "Get Data" command
  Wire.endTransmission();
  //time delays required by HMC6352 upon receipt of the command
  //Get Data. Compensate and Calculate New Heading : 6ms
  delay(6);

  Wire.requestFrom(SlaveAddress, 2); //get the two data bytes, MSB and LSB

  //"The heading output data will be the value in tenths of degrees
  //from zero to 3599 and provided in binary format over the two bytes."
  byte MSB = Wire.read();
  byte LSB = Wire.read();

  heading = ((MSB << 8) + LSB) / 10;
  sHeading = dtostrf(heading, 1, 4, Buffer);
}

float Compass::getHeading(){return heading;}
String Compass::getSHeading(){return sHeading;}