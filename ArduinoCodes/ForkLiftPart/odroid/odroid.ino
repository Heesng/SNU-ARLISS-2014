void setup(){
 pinMode(45,OUTPUT);//direction
 pinMode(35,OUTPUT);//enable
 pinMode(37,OUTPUT);//pwm 
}

void loop(){
  digitalWrite(35,LOW);
  digitalWrite(45,HIGH);
  digitalWrite(37,HIGH);  
}
