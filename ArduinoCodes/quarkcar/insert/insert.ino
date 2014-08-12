#include<Servo.h>

void insert(float a);

Servo Carsteer;
float rsangle = 0;
int motor = 2;

void setup(){
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
  if(Serial.available()>0){
    rsangle = Serial.read();
    if(rsangle<-45||rsangle>45){
      Serial.println("Wrong range");
    }
    delay(10);
  }
  insert(rsangle);
}

void insert(float rsangle){
  if(-10<rsangle&&rsangle<10){
    analogWrite(motor, 190);
  }
  else if(-45<rsangle&&rsangle<10){
    Carsteer.write(60);
    analogWrite(motor, 180);
    Carsteer.write(120);
    analogWrite(motor, 180);
  }
  else if(10<rsangle&&rsangle<45){
    Carsteer.write(120);
    analogWrite(motor, 180);
    Carsteer.write(60);
    analogWrite(motor, 180);    
  }
}

