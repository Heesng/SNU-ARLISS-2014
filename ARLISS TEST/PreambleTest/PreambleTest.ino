void setup(){
  
  Serial.begin(9600);
  Serial1.begin(1200);
}

void loop(){
  
  //String preamble = String(0xFFFFFF);
  int x = 0xFF ;
  Serial1.println(x,HEX);
  
  //Serial1.println(preamble);
  delay(50);
}
