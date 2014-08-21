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
int preloc = 200;

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
    if(val!=200){
      preloc = val;
    }
    if(val==200&&preloc!=200){
      insert(201); 
    }      
    insert(val);
    Serial.println(val);
    delay(1000);
}

void insert(float rsangle){
  if(-40<rsangle&&rsangle<40){
    Carsteer.write(90);
    delay(100);
    analogWrite(motor, 185);
    delay(100);
  }
  else if(40<rsangle&&rsangle<120){
    Carsteer.write(70);
    delay(100);
    analogWrite(motor, 175);
    delay(300);
    Carsteer.write(110);
    delay(100); 
    analogWrite(motor, 175);
    delay(300);
    Carsteer.write(90);
    delay(100);
    analogWrite(motor, 185);
    delay(400);
  }
  else if(-120<rsangle&&rsangle<-40){
    Carsteer.write(110);
    delay(100);
    analogWrite(motor, 175);    
    delay(300);
    Carsteer.write(70);
    delay(100);
    analogWrite(motor, 175);
    delay(300);
    Carsteer.write(90);
    delay(100);
    analogWrite(motor, 185);
    delay(400);    
  }
  else if (rsangle == 200){
    Carsteer.write(90);
    delay(100);
    analogWrite(motor, 181);
    delay(100);
  }
  else{
    Carsteer.write(90);
    delay(100);
    analogWrite(motor, 175);
    delay(100);
  }
}
