#include "Arduino.h"
#include "DCS.h"

DCS::DCS(){}

void DCS::mergeData(String module_, String state_, String slat_, String slng_, String shgt_, String sheading_){

  rfData = module_ +"," +state_ +"," + slat_ +"," + slng_ +"," + shgt_ +"," + sheading_;
  sdData = module_ +"\t"+state_ +"\t"+ slat_ +"\t"+ slng_ +"\t"+ shgt_ +"\t"+ sheading_;

}

int DCS::readPck(String pck_){

  rcvPCK = pck_;
  char comma = ',';

  int i = pck_.indexOf(comma);
  module = pck_.substring(0,i-1);

  int j = pck_.indexOf(comma,i+1);
  sate = pck_.substring(i+1,j-1);

  i = pck_.indexOf(comma,j+1);
  sLat = pck_.substring(j+1,i-1);

  j= pck_.indexOf(comma,i+1);
  sLng = pck_.substring(i+1,j-1);

  i = pck_.indexOf(comma,j+1);
  sHgt = pck_.substring(j+1,i-1);

  j= pck_.indexOf(comma,i+1);
  sHeading = pck_.substring(i+1,j-1);

  char buf0[sLat.length()];
  char buf1[sLng.length()];
  char buf2[sHgt.length()];
  char buf3[sHeading.length()];

  if(sLat.length() != 0)
  {
  	sLat.toCharArray(buf0,sLat.length());
  	lat = atof(buf0); 
    sLng.toCharArray(buf1,sLng.length());
    lng = atof(buf1); 
    sHgt.toCharArray(buf2,sHgt.length());
    hgt = atof(buf2);
    sHeading.toCharArray(buf2,sHeading.length());
    heading = atof(buf3);

    return 0;
  }

  else{
    lat = 0;
    lng = 0;
    hgt = 0;
    return -1;
  }
}

word16 DCS::Checksum(String A){

  word32 buff_len = A.length();
  char buff[buff_len];
  A.toCharArray(buff,buff_len);

  register word32 sum = 0;
 // Main summing loop
  while(buff_len > 1)
  {
    sum = sum + *((word16 *) buff);
    *((word16 *) buff) = *((word16 *) buff) + 1 ;
    buff_len = buff_len - 2;
  }
   // Add left-over byte, if any
  if (buff_len > 0)
   sum = sum + *((byte *) buff);
   // Fold 32-bit sum to 16 bits
 while (sum>>16)
   sum = (sum & 0xFFFF) + (sum >> 16);
 return(~sum);
}



String DCS::getRfData(){return rfData;}
String DCS::getSdData(){return sdData;}
String DCS::getSLat(){return sLat;}
String DCS::getSLng(){return sLng;}
String DCS::getSHgt(){return sHgt;}
String DCS::getSHeading(){return sHeading;}

double DCS::getLat(){return lat;}
double DCS::getLng(){return lng;}
double DCS::getHgt(){return hgt;}
double DCS::getHeading(){return heading;}
