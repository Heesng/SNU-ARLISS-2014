void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
}

void loop()
{
  Serial1.println("from 301 by module a");
  Serial2.println("from 301 by module b");
  while(Serial1.available())
  {
    char c = Serial1.read();
    Serial.write(c);
  }
  while(Serial2.available())
  {
    char c = Serial2.read();
    Serial.write(c);
  }
}
