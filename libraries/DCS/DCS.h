//Code for 13-14 ARLISS Bread System.
//First written at 2014.07.06
//Latest renew at 2014.07.06

#ifndef DCS_h
#define DCS_h

#include "Arduino.h"

typedef unsigned char      byte;    // byte(8-bit) is a char
typedef unsigned short int word16;  // 16-bit word is a short int
typedef unsigned int       word32;  // 32-bit word is an int


class DCS{
public:
	DCS();
	
	void mergeData(String module_, String state_, String slat_, String slng_, String shgt_, String sheading_);

	int readPck(String pck_);

	String getRfData();
	String getSdData();


	String getModule();
	String getState();

	String getSLat();
	String getSLng();
	String getSHgt();
	String getSHeading();
	double getLat();
	double getLng();
	double getHgt();
	double getHeading();

private:
	word16 Checksum(String A);

	/*
	merged data from this module.
	*/
	String rfData;
	String sdData;

	/*
	received data
	*/
	String rcvPCK;

	String module;
	String state;

	String sLat;
	String sLng;
	String sHgt;
	String sHeading;
	double lat;
	double lng;
	double hgt;
	double heading;
};

#endif
