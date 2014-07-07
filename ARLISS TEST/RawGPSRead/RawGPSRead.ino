void setup()
{
  Serial.begin(9600);
  Serial2. begin(9600);
}
void loop()
{    
    char ch ;
    while(Serial2.available())
      {
        ch = Serial2.read();
        Serial.write(ch);
      }
}
