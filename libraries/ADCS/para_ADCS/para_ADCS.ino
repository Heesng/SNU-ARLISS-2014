#include <ADCS.h>

ADCS adcs;

float a;
int i = 0, j = 0;

void setup(){
	Serial.begin(9600);
	delay(3000);
}

void loop(){
	if(i==0){
		i = adcs.reelPara();
	}
	j = adcs.renew();
	if(j==1){
		adcs.control();
	}
}