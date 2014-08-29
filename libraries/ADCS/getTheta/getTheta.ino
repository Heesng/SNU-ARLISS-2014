#include <ADCS.h>

ADCS adcs;

float a;
int i = 0, j = 0;

void setup(){
	Serial.begin(9600);
	delay(3000);
}

void loop(){
	
	j = adcs.renew();
	int a = adcs.gettheta();

	Serial.println(a);
	delay(100);
}