#ifndef CTS_H
#define CTS_H

class CTS
{
  public:
    CTS();
    int xcoord(int num); //n'th color's xcoord(-119~120, 200 for not detected or not communicated)
	int location(); //real return value(-119~120, 200 for not detected)	
	int size(int num);//n'th color's size(1-255, 0 for not detected, 200 for not communicated)
};
#endif