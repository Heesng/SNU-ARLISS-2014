#include<Servo.h>

void insert(float a);

Servo Carsteer;
float rsangle = 0;
int motor = 2;

void setup(){
  delay(3000);
  Serial.begin(9600);
  Carsteer.attach(3);
  pinMode(2,OUTPUT);
  delay(1000);
  for(int i = 56; i <= 104 ; i++){
    analogWrite(motor, i);
    delay(10);
  }
  analogWrite(motor, 170);
  delay(1000);
  analogWrite(motor, 190);
  delay(1000);
}

void loop(){
  analogWrite(motor, 186);
  delay(2000);
}

void serialEvent(){
  if(Serial.available()){
      rsangle = Serial.parseInt();
      Serial.print(rsangle);      
  }
  insert(rsangle);
}

void insert(float rsangle){
  if(-10<rsangle&&rsangle<10){
    analogWrite(motor, 210);
    delay(2000);
  }
  else if(-45<rsangle&&rsangle<-10){
    Carsteer.write(60);
    analogWrite(motor, 150);
    delay(1000);
    Carsteer.write(120);
    analogWrite(motor, 150);
    delay(1000);
  }
  else if(10<rsangle&&rsangle<45){
    Carsteer.write(120);
    analogWrite(motor, 150);    
    delay(1000);
    Carsteer.write(60);
    analogWrite(motor, 160);
    delay(1000);    
  }
}

