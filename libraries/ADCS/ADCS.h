
#ifndef ADCS_h
#define ADCS_h

#include "Arduino.h"

class ADCS{
public:
	ADCS();
	int renew();
	void control();
	int reelPara();
	float gettheta();

private:
	int pwmPin1;
	int disablePin1;
	int dirPin1;

	int pwmPin2;
	int disablePin2;
	int dirPin2;
 	//Acclerometer Pin definitions
 	int XAccPin;
 	int YAccPin;
 	int ZAccPin;

	//Data Variables
 	//Attitude
 	float Acc[3];
 	float theta;
 	float dtheta;
 	float theta0;

	//Control
	float wr;
	int wrInt;
	int PGain;
	int DGain;

	long reeltime;
	int reelcheck;
};

#endif


