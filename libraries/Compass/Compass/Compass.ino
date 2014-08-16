#include <Wire.h>
#include <Compass.h>

Compass comp;

void setup(){
  Serial.begin(9600);
  Wire.begin();
}

void loop(){
  comp.renew();
  Serial.print(comp.getSHeading());
  Serial.println("degree");
  delay(100);
}