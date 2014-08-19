#include "CTS.h"
CTS CTS1;
int val=0;
int data=0;
int center=120;
void setup()
{
  Serial.begin(115200);
  Serial3.begin(115200);

}

void loop()
{
  if(Serial.available())
  {
    Serial.read();
    val=CTS1.location();
    Serial.println(val);
    delay(2000);
  }
}
