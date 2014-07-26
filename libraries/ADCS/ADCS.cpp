#include "Arduino.h"
#include "ADCS.h"

ADCS::ADCS(){
	pwmPin1 = 2;
	enablePin1 = 4;
	dirPin1 =3;

	pwmPin2 = 5;
	enablePin2 = 7;
	dirPin2 = 6;

	XAccPin = 0;
	YAccPin = 1;
	ZAccPin = 2;
	
	Acc[3];
	theta = 0;
	dtheta = 0;
	theta0 =0;

	PGain=6;
	DGain=0;
	
	pinMode(enablePin1, OUTPUT);
	pinMode(dirPin1, OUTPUT);
	pinMode(pwmPin1, OUTPUT);
	pinMode(enablePin2, OUTPUT);
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
	if(theta < 95 && theta > 75) return 0;	//when sensor is erect
	else return 1;
}


void ADCS::control(){
	int i = 1;

	while(i==1){

	//Attitude Control
	//Object attitude of 
	//Enable Motor
		digitalWrite(enablePin2, LOW);
		
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
	digitalWrite(enablePin2, HIGH);

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

	digitalWrite(enablePin1, LOW);
	
	if(theta>140||theta<-140){
		theta=140;
	}
	if(theta<84){
		digitalWrite(dirPin1, HIGH);
	}
	else {
		digitalWrite(dirPin1, LOW);
	}

	/*
	for(int i =0; i<255; i++){
		analogWrite(pwmPin1, i);
		delay(40);
	}*/


		analogWrite(pwmPin1, 250);
		delay(8000);
		digitalWrite(enablePin1, HIGH);

		return 1;
	}



	float ADCS::gettheta(){return theta;}