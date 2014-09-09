#include "Arduino.h"
#include "ADCS.h"

ADCS::ADCS(){
	/*
	//for parachute reeling motor
	pwmPin1 = 2;
	disablePin1 = 4;
	dirPin1 =3;

	//for attitude controlling motor
	pwmPin2 = 5;
	disablePin2 = 7;
	dirPin2 = 6;
	*/
	

	//for parachute reeling motor
	pwmPin1 = 5;
	disablePin1 = 7;
	dirPin1 =6;

	//for attitude controlling motor
	pwmPin2 =2;
	disablePin2 = 4;
	dirPin2 = 3;
	

	XAccPin = 0;
	YAccPin = 1;
	ZAccPin = 2;
	
	Acc[3];
	theta = 0;
	dtheta = 0;
	theta0 =0;

	PGain=8;
	DGain=0;

	reeltime = 0;
	reelcheck = 0;
	
	pinMode(disablePin1, OUTPUT);
	pinMode(dirPin1, OUTPUT);
	pinMode(pwmPin1, OUTPUT);
	pinMode(disablePin2, OUTPUT);
	pinMode(dirPin2, OUTPUT);
	pinMode(pwmPin2, OUTPUT);

}

int ADCS::renew(){
	//Attitude Determination
 	//Read Sensor Data
	for(int i=0; i<4; i++){
		Acc[i] = analogRead(i)-333;
	}
	//Calculate 1-D Attitude
	//Reference input for object attitude is theta = 0
	theta = (180/3.14)*atan2(Acc[2], Acc[1])-0.5;
	dtheta = theta - theta0;
	theta0 = theta;
	Serial.println(gettheta());

	if(millis() > reeltime + 10500 && reelcheck==1){
		digitalWrite(disablePin1, HIGH);
	}

	if(theta < 90 && theta > 78) return 0;	//when sensor is erect, 84 is median

	else return 1;
}


void ADCS::control(){
	int i = 1;
	long t = millis();
	while(i==1){
		if(t+3000 < millis()){break;}

	//Attitude Control
	//Object attitude of 
	//Enable Motor
		digitalWrite(disablePin2, LOW);

		if(theta>140||theta<-140){
			theta=140;
		}
		if(theta<84){
			digitalWrite(dirPin2, LOW);
			wr =-1*PGain*((theta-84)+ DGain*dtheta);
		}
		else {
			digitalWrite(dirPin2, HIGH);
			wr = (PGain*((theta-84)+ DGain*dtheta));
		}
		wrInt = (int) wr;
		if(wrInt>255){
			wrInt=255;
		}
		analogWrite(pwmPin2, wrInt);
		i = renew();
	}
	digitalWrite(disablePin2, HIGH);

}

int ADCS::reelPara(){
	//Attitude Determination
 	//Read Sensor Data
	for(int i=0; i<4; i++){
		Acc[i] = analogRead(i)-333;
	}
	//Calculate 1-D Attitude
	//Reference input for object attitude is theta = 0
	theta = (180/3.14)*atan2(Acc[2], Acc[1])-0.5;
	dtheta = theta - theta0;
	theta0 = theta;
	//Attitude Control
	//Object attitude of 
	//Enable Motor

	digitalWrite(disablePin1, LOW);

	if(theta>140||theta<-140){
		theta=140;
	}
	if(theta<84){
		digitalWrite(dirPin1, HIGH);
	}
	else {
		digitalWrite(dirPin1, LOW);
	}

	analogWrite(pwmPin1, 250);
	reeltime = millis();
	reelcheck = 1;
	return 1;
}

float ADCS::gettheta(){return theta;}