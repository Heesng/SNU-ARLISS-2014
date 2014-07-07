//Code for 13-14 ARLISS Bread System.
//First written at 2014.07.06
//Latest renew at 2014.07.06

#ifndef DCS_h
#define DCS_h

#include "Arduino.h"

class DCS{
public:
	DCS();
	
	void makePck(String lat_, String lng_, String hgt_);
	int readPck(String pck_);

	String getPck();
	double getLat();
	double getLng();
	double getHgt();
	String getSLat();
	String getSLng();
	String getSHgt();

private:
	String packet;
	String sLat;
	String sLng;
	String sHgt;
	double lat;
	double lng;
	double hgt;
	
};

#endif
