
#ifndef ADCS_h
#define ADCS_h

#include "Arduino.h"


class ADCS{
public:
	ADCS();
	void control();
	int checkAtt();


private:
	int pwmPin = 7;
 	int enablePin = 5;
 	int dirPin = 6;
 	//Acclerometer Pin definitions
 	int XAccPin = 0;
 	int YAccPin = 1;
 	int ZAccPin = 2;

	//Data Variables
 	//Attitude
 	float Acc[3];
 	float theta;
 	float dtheta;
 	float theta0;

	//Control
	 int wr;
	 int PGain=5;
	 int DGain=2;


};

#endif


