#include "Arduino.h"
#include "RF.h"

RF::RF(){  
}

void RF::sendPck(String A)
{
  Serial1.write(0xFFFFFF);
  word16 chescksum = Checksum(A);
  char cs[2];

  cs[0] = chescksum & 0xFF;
  cs[1] = (chescksum >> 8) & 0xFF;

  A = "SNU" + A + cs[0] + cs[1];
//  Serial.println(A);

  Serial1.println(A);
  Serial1.println(A);
  Serial1.println(A);
}

int RF::receivePck(String & A)
{
	char c;
  while(Serial1.available()>10){
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

          //Serial.println(A);
          char cs[2];
          cs[0] = A.charAt(A.length()-3);
          cs[1] = A.charAt(A.length()-2);
          A = A.substring(0,A.length()-3);

          word16 chescksum = Checksum(A);
          char cs_[2];

          cs_[0] = chescksum & 0xFF;
          cs_[1] = (chescksum >> 8) & 0xFF;

          Serial.write(cs[0]);
          Serial.write('\t');

          Serial.write(cs[1]);
          Serial.write('\t');

          Serial.write(cs_[0]);
          Serial.write('\t');

          Serial.write(cs_[1]);
          Serial.write('\n');

          if(cs[0] == cs_[0] && cs[1]==cs_[1]) return 0; //received without noise
          else return -1; //received with noise
        }
      }
    }
  }return 1;//packet is not received.
}

/*
void RF::sendACK(){

}
*/

word16 RF::Checksum(String A){

  word32 buff_len = A.length();
  char buff[buff_len];
  A.toCharArray(buff,buff_len);

  register word32 sum = 0;
 // Main summing loop
  while(buff_len > 1)
  {
    sum = sum + *((word16 *) buff);
    *((word16 *) buff) += 1;

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