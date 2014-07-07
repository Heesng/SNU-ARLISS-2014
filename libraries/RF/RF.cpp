#include "Arduino.h"
#include "RF.h"

RF::RF(){
  
}

void RF::SendPacket(String A)
{
  Serial1.write(0xFFFFFF);
  A = "SNU" + A;
  Serial1.println(A);
}

int RF::ReceivePacket(String & A)
{
	char c;

	if(Serial1.available()>0)
  {
    c = Serial1.read();delay(5);
    if(c == 'S'){delay(5);
      c = Serial1.read();delay(5);
      if(c == 'N'){delay(5);
      	c = Serial1.read();delay(5);
      		if(c == 'U'){delay(5);
      			c = Serial1.read();delay(5);
            while(c != '\n')
            {
              A += c;delay(5);
              c = Serial1.read();delay(5);
            }
            return 0;
          }
        }
      }
    }
    return -1;
}