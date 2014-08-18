String A,B,C,D;
char c;



void setup(){

	Serial.begin(9600);
	Serial1.begin(9600);
	Serial2.begin(9600);
	Serial3.begin(9600);

	Serial.println("Let's begin");
}


void loop(){

	while(Serial.available()>0)
	{
		c = Serial.read();delay(5);
		A += c;
	}

	while(Serial1.available()>0)
	{
		c = Serial1.read();delay(5);
		B += c;
	}

	while(Serial2.available()>0)
	{
		c = Serial2.read();delay(5);
		C += c;
	}

	while(Serial3.available()>0)
	{
		c = Serial3.read();delay(5);
		D += c;
	}

	if(A != ""){
		Serial.print("from 0 to 1: ");
		Serial.println(A);

		Serial.print("from 1 to 2: ");
		Serial.println(A);

		Serial1.print(A);
		A = "";
	}
	
	if(B != ""){
		Serial.print("from 1 to 2: ");
		Serial.println(B);

		Serial1.print(B);
		B = "";
	}

	if(C != ""){
		Serial.print("from 2 to 3: ");
		Serial.println(C);

		Serial2.print(C);
		C = "";
	}

	if(D != ""){
		Serial.print("from 3 to 1: ");
		Serial.println(D);

		Serial3.print(D);
		D = "";
	}


}