//This is a program for 

///Pin Definitions
 //DCMotor Driver Pin definitions
 int pwmPin = 7;
 int enablePin = 5;
 int dirPin = 6;
 //Acclerometer Pin definitions
 int XAccPin = 0;
 int YAccPin = 1;
 int ZAccPin = 2;

//Data Variables
 //Attitude
 float Acc[3];
 float theta;
 float dtheta;
 float theta0;

//Control
 int wr;
 int PGain=5;
 int DGain=2;

void setup(){

Serial.begin(9600);
 //Pin Definitions
 //Motor Driver Pins
 pinMode(enablePin, OUTPUT);
 pinMode(dirPin, OUTPUT);
 pinMode(pwmPin, OUTPUT);


 }

void loop(){

	//Attitude Determination
 	//Read Sensor Data
 	for(int i=0; i<4; i++){
		Acc[i] = analogRead(i)-333;
	}
	//Calculate 1-D Attitude
	//Reference input for object attitude is theta = 0
	theta = (180/3.14)*atan2(Acc[1], Acc[2])-0.5;
	dtheta = theta - theta0;
	theta0 = theta;

	//Attitude Control
	//Object attitude of 
	//Enable Motor
	digitalWrite(enablePin, LOW);
	if(theta>0){
	  digitalWrite(dirPin, HIGH);
	  wr = PGain*theta+ DGain*dtheta;
	}
	else {
	   digitalWrite(dirPin, LOW);
	   wr = -1*(PGain*theta+ DGain*dtheta);
	}

	analogWrite(pwmPin, wr);

delay(100);

 }

