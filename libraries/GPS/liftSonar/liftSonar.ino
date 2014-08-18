//Example for GPS module.
//GPS module uses Seraial2 in this library.

#include <GPS.h>

int sonar1 = 22;
int sonar2 = 23;
int sonar3 = 24;

GPS gpsEx;

void setup(){
  Serial.begin(9600);
  pinMode(sonar1,INPUT); // 센서 Echo 핀
  pinMode(sonar2,INPUT); // 센서 Echo 핀
  pinMode(sonar3,INPUT); // 센서 Echo 핀
}

void loop(){

  int sonardist1 = gpsEx.liftsonar(sonar1);
  int sonardist2 = gpsEx.liftsonar(sonar2);
  int sonardist3 = gpsEx.liftsonar(sonar3);

  Serial.print("sonar1 distance: ");
  Serial.println(sonardist1);
  Serial.print("sonar2 distance: ");
  Serial.println(sonardist2);
  Serial.print("sonar3 distance: ");
  Serial.println(sonardist3);


}