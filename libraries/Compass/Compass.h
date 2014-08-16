#include "Arduino.h"
#include <Wire.h>


#ifndef Compass_h
#define Compass_h

class Compass{
public:
	Compass();
	void renew();
	float getHeading();
  String getSHeading();
private:
	int SlaveAddress;
	int ReadAddress; //"A" in hex, A command is:
	float heading;
  String sHeading;
  char Buffer[5];
};

#endif