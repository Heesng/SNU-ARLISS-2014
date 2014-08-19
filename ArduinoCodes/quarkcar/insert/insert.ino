#include<Servo.h>
#include<CTS.h>

void insert(float a);

CTS CTS1;
Servo Carsteer;
float rsangle = 0;
int motor = 2;
int val = 0;
int data=0;
int center=120;

void setup(){
  delay(3000);
  Serial.begin(115200);
  Serial3.begin(115200);
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
    val = CTS1.location();
    insert(val);
    Serial.println(val);
    delay(100);
}

void insert(float rsangle){
  if(-10<rsangle&&rsangle<10){
    analogWrite(motor, 200);
    delay(200);
  }
  else if(-120<rsangle&&rsangle<-10){
    Carsteer.write(60);
    analogWrite(motor, 170);
    delay(100);
    Carsteer.write(120);
    analogWrite(motor, 170);
    delay(100);
  }
  else if(10<rsangle&&rsangle<120){
    Carsteer.write(120);
    analogWrite(motor, 170);    
    delay(100);
    Carsteer.write(60);
    analogWrite(motor, 170);
    delay(100);    
  }
}

