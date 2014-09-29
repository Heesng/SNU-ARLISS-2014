 int motor = 2;           // the pin that the LED is attached to

// the setup routine runs once when you press reset:
void setup()  { 
  pinMode(2,OUTPUT);
//  int myEraser = 7;             // this is 111 in binary and is used as an eraser
//  TCCR3B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
//  int myPrescaler = 2;         // this could be a   number in [1 , 6]. In this case, 3 corresponds in binary to 011.   
//  TCCR3B |= myPrescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
//  // declare pin 9 to be an output:
  delay(1000);
//  analogWrite(motor, 210);
//  delay(10);
  for(int i = 56; i <= 104 ; i++){
    analogWrite(motor, i);
    delay(10);
  }
  analogWrite(motor, 170);
  delay(1000);
  analogWrite(motor, 190);
  delay(1000);
  analogWrite(motor, 181);
  delay(1000);
  
} 
//179~184 ->neutral
// the loop routine runs over and over again forever:
void loop()  {
    analogWrite(motor, 188);
    delay(5000);
        analogWrite(motor, 175);
    delay(5000);
        analogWrite(motor, 181);
    delay(5000);
        analogWrite(motor, 175);
    delay(5000);
        analogWrite(motor, 188);
    delay(5000);
    //analogWrite(motor, 230);
    //delay(10);               
}

