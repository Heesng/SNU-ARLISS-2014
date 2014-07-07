//Example for GPS module.
//GPS module uses Seraial2 in this library.

#include <GPS.h>

GPS gpsEx;

void setup(){
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop(){
  gpsEx.renewGPS();
  double lat = gpsEx.getLat();
  double lng = gpsEx.getLng();
  double hgt = gpsEx.getHgt();
  String LAT = gpsEx.getSLat();
  String LNG = gpsEx.getSLng();
  String HGT = gpsEx.getSHgt();

  delay(1000);
}
