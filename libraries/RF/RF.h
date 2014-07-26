//Code for 13-14 ARLISS Bread System.
//First written at 2014.6.18
//Latest renew at 2014.07.03
//RF module uses Seraial1 in this library.

#ifndef RF_h
#define RF_h

#include "Arduino.h"

typedef unsigned char      byte;    // byte(8-bit) is a char
typedef unsigned short int word16;  // 16-bit word is a short int
typedef unsigned int       word32;  // 32-bit word is an int

class RF{
public:
	RF();
	void sendPck(String A);
	int receivePck(String & A);
private:
	word16 Checksum(String A);
///	void sendACK();

};

#endif