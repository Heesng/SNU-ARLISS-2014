char c=' ';
char d=' ';

void setup(){
  Serial.begin(115200);
  Serial3.begin(115200);
}

void loop(){
  if(Serial3.available())
  {
    c=Serial3.read();
    if(c=='a'||c=='b'||c=='c'||c=='e')
      Serial.write(c); // a:left, b:middle, c:right, e:no target
    else if(c=='0')
      Serial.print("err, please reboot");
      //go to back(reboot odroid)
  }
  if(Serial.available())
  {
    d=Serial.read();
    Serial.write(d);
  }
}
