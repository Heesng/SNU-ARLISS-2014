//Code for 13-14 ARLISS Bread System.
//First written at 2014.4.01
//Latest renew at 2014.07.03
//GPS module uses Seraial2 in this library.

#ifndef GPS_h
#define GPS_h

#include "Arduino.h"

class GPS{
public:

  GPS();
  int renew();
  long liftsonar(int pinnum);

  
  double getLat();
  double getLng();
  double getHgt();
  double getDeltaH();

  String getSLat();
  String getSLng();
  String getSHgt();

  int count;

private:
  void BufIn(String &buf);  // Get GPS data from GPS module and store the data into GPSBuf
  int getPhrase(String &momBuf, String &sonBuf); //Get the phrase that includes longitude and latitude data.
  String getLat(String Buf);
  String getLong(String Buf);
  String getHigh(String Buf);
  void StoF();
  int spellCheck(String Buf);
  
  String GPSBuf; //Whole GPS data buffer
  String GPSPhr; //GPS phrase buffer
  
  String fallV; //falling velocity

  double latitude;
  double longitude;
  double height;
  double deltaH;
  String sLatitude;
  String sLongitude;
  String sHeight;
};

#endif