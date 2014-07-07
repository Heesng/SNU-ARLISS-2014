#include "Arduino.h"
#include "DCS.h"



DCS::DCS(){}

void DCS::makePck(String slat_, String slng_, String shgt_){
		
		packet = slat_ +","+ slng_ +","+ shgt_;
}

int DCS::readPck(String pck_){

  packet = pck_;
	char comma = ',';
	
	int i = pck_.indexOf(comma);
	sLat = pck_.substring(0,i-1);

	int j = pck_.indexOf(comma,i+1);
	sLng = pck_.substring(i+1,j-1);

	i = pck_.indexOf(comma,j+1);
	sHgt = pck_.substring(j+1,i-1);


	char buf0[sLat.length()];
  	char buf1[sLng.length()];
  	char buf2[sHgt.length()];

  	if(sLat.length() != 0)
  	{
  		sLat.toCharArray(buf0,sLat.length());
  		lat = atof(buf0); 

    	sLng.toCharArray(buf1,sLng.length());
    	lng = atof(buf1); 

    	sHgt.toCharArray(buf2,sHgt.length());
    	hgt = atof(buf2);

      return 0;
  	}
  	else{
    	lat = 0;
    	lng = 0;
    	hgt = 0;

      return -1;
  	}
}


String DCS::getPck(){ return packet;}

double DCS::getLat(){return lat;}
double DCS::getLng(){return lng;}
double DCS::getHgt(){return hgt;}
String DCS::getSLat(){return sLat;}
String DCS::getSLng(){return sLng;}
String DCS::getSHgt(){return sHgt;}