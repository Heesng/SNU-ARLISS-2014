
ADCS::ADCS(){
	 pinMode(enablePin, OUTPUT);
	 pinMode(dirPin, OUTPUT);
	 pinMode(pwmPin, OUTPUT);
}


void ADCS::control(){
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
}


int ADCS::checkAtt(){
	if(25 < theta && theta < -25)
	{ return 0;}
	else
		return -1;
}
