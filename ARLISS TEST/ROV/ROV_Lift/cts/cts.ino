#include "CTS.h"
CTS CTS1;
int val=0;
int data=0;
int center=120;
void setup()
{
  Serial.begin(9600);
  Serial3.begin(9600);

}

void loop()
{
  if(Serial.available())
  {
    Serial.read();
    val=CTS1.location();
    Serial.println(val);
  }
}
