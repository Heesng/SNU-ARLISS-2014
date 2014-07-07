//Code for 13-14 ARLISS Bread System.
//First written at 2014.6.18
//Latest renew at 2014.07.03
//RF module uses Seraial1 in this library.

#ifndef RF_h
#define RF_h

#include "Arduino.h"


class RF{
public:
	RF();
	void SendPacket(String);
	int ReceivePacket(String & A);

};

#endif