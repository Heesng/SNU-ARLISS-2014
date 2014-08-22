#include "Arduino.h"
#include "GPS.h"

GPS::GPS(){
  count = 0;
  latitude = 0;
  longitude = 0;
  height = 0;
  sLatitude = "0";
  sLongitude = "0";
  sHeight = "0";
}

int GPS::renew(){

  BufIn(GPSBuf);
  int i = getPhrase(GPSBuf, GPSPhr);
  if(i == 1) return 0;  //not renewed 
  else{
    String tempLat = getLat(GPSPhr);
    String tempLng = getLong(GPSPhr);
    String temphgt = getHigh(GPSPhr);

    int a = spellCheck(tempLat);
    int b = spellCheck(tempLng);
    int c = spellCheck(temphgt);

    if(a == 0) sLatitude = tempLat;
    if(b == 0) sLongitude = tempLng;
    if(c == 0) sHeight = temphgt;
    StoF();
    return 1;

  }
}

  int GPS::liftsonar(int pinnum){
    int duration = pulseIn(pinnum,LOW);
    return duration*170/10000;
  }


  double GPS::getLat(){return latitude;}
  double GPS::getLng(){return longitude;}
  double GPS::getHgt(){return height;}
  double GPS::getDeltaH(){return deltaH;}

  String GPS::getSLat(){return sLatitude;}
  String GPS::getSLng(){return sLongitude;}
  String GPS::getSHgt(){return sHeight;}



void GPS::BufIn(String & buf){  // Get GPS data from GPS module and store the data into GPSBuf
  while(Serial2.available()>0)
  {
    char c;
    c = Serial2.read();
    buf += c;
  }
}

int GPS::getPhrase(String & momBuf, String & sonBuf){ //Get the phrase that includes longitude and latitude data.
  char Init = '$';
  String AddrIndex = "GPGGA";
  char Delimiter = '\n';
  int i = 0;
  int j = 0;
  int e = 0;// error check
  i = momBuf.lastIndexOf(Init);
  i = momBuf.lastIndexOf(Init, i-1);  
  
  while(1){

    if(i == -1){ //In the case that GPS data is not received 
      e = 1;
      break;
    }
    else if(momBuf.substring(i+1,i+6) != AddrIndex){  
      i = momBuf.lastIndexOf(Init, i-1);
    }
    else{
      j = momBuf.indexOf(Delimiter,i+1);
      sonBuf = momBuf.substring(i,j);
      momBuf = momBuf.substring(j+1);
      break;
    }
  }
  return e;
}

String GPS::getLat(String Buf){
  char comma = ',';
  String temp;
  int i = 0;
  int j = 0;
  i = Buf.indexOf(comma);
  i = Buf.indexOf(comma,i+1);
  j = Buf.indexOf(comma,i+1);
  if(i + 1 == j)  return 0;
  else{
    temp = Buf.substring(i+1,j);
    return temp;
  }
}

String GPS::getLong(String Buf){
  char comma = ',';
  String temp;
  int i = 0;
  int j = 0;
  i = Buf.indexOf(comma);
  i = Buf.indexOf(comma,i+1);
  i = Buf.indexOf(comma,i+1);
  i = Buf.indexOf(comma,i+1);
  j = Buf.indexOf(comma,i+1);
  if(i + 1 == j)  return 0;
  else{
    temp = Buf.substring(i+1,j);
    return temp;
  }
}

String GPS::getHigh(String Buf){
  char comma = ',';
  String temp;
  int i = 0;
  int j = 0;
  i = Buf.indexOf(comma);
  i = Buf.indexOf(comma,i+1);
  i = Buf.indexOf(comma,i+1);
  i = Buf.indexOf(comma,i+1);
  i = Buf.indexOf(comma,i+1);
  i = Buf.indexOf(comma,i+1);
  i = Buf.indexOf(comma,i+1);
  i = Buf.indexOf(comma,i+1);
  i = Buf.indexOf(comma,i+1);
  j = Buf.indexOf(comma,i+1);
  if(i + 1 == j)  return 0;
  else{
    temp = Buf.substring(i+1,j);
    return temp;
  }
}

void GPS::StoF(){
  char buf0[sLatitude.length()];
  char buf1[sLongitude.length()];
  char buf2[sHeight.length()];
  float temp;

  if(sLatitude.length() != 0)
  {
    sLatitude.toCharArray(buf0,sLatitude.length());
    latitude = atof(buf0); 

    sLongitude.toCharArray(buf1,sLongitude.length());
    longitude = atof(buf1); 

    sHeight.toCharArray(buf2,sHeight.length());
    temp = height;
    height = atof(buf2);
    deltaH = height - temp; 
  }
  else{
    latitude = 0;
    longitude = 0;
    height = 0;
  }
}

int GPS::spellCheck(String Buf){

  int n = Buf.length();
  int i = 0;
  char c;
  int result = 0;

  while(i<n){
    c = Buf.charAt(i);
    if(('0'>c || c>'9')&&c!='.'){return -1;}
    i++;
  }
  return 0;
}