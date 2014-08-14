void setup()
{
  Serial.begin(9600);
  Serial2. begin(9600);
}
void loop()
{    
    char ch;
    if(Serial2.available())
      {
        ch = Serial2.read();
        Serial.write(ch);
      }
}
