#include <ADCS.h>
#include <GPS.h>
#include <RF.h>
#include <SD.h>
#include <math.h>
#include <Wire.h>

ADCS adcs;
GPS gps;
RF rf;

String module = "S";
int chipSelect = 8;

int ledPin1 = 9;
int ledPin3 = 11;
int ledpin5 = 13;

void mergeData(String module_, String state_, String slat_, String slng_, String shgt_, String sheading_);
int readPck(String pck_);
void compassRenew();
void sdWrite(String dataString);


int a,d,g=0, r=0; //adcs,dcs,gps,rfrcv

String state = "";
String rcvPck = "";

String rfData = "";
String sdData = "";

String rmodule = " ";//Received module
String rstate = " ";//Received state
String rslat = " ";//Received String latitude
String rslng = " ";//Received String longitude
String rshgt = " ";//Received String height
String rsheading = " ";//Received String heading

float rlat = 0;//Received latitude
float rlng = 0;//Received longitude
float rhgt = 0;//Received height
float rheading = 0;//Received heading


int SlaveAddress = 0x42;
int ReadAddress = 0x41; //"A" in hex, A command is: 

int headingValue;
float headingSum;
float headingInt;

String sHeading;
char Buffer[5];

void setup(){
	state = "S";
	Serial.begin(9600);
	Serial1.begin(1200);
	Serial2.begin(9600);

  /*
  compass setting
  */
  SlaveAddress = SlaveAddress >> 1; // I know 0x42 is less than 127, but this is still required
  Wire.begin();

  /*
  sd card setting
  */
  pinMode(10, OUTPUT);
  SD.begin(chipSelect);

  g = 0;
  while(g != 1){ g = gps.renew();}

}

void loop(){

  g = gps.renew();

  /*
  Compass renew
  */
  compassRenew();

  /*
  RF sending code
  */
  if(g==1){
    mergeData(module,state,gps.getSLat(),gps.getSLng(),gps.getSHgt(),sHeading);
    rf.sendPck(rfData);
    sdWrite(sdData);
    Serial.println(rfData);
  }
  /*
  RF receive code
  */
  r = rf.receivePck(rcvPck);
  if(r==0){
    readPck(rcvPck);
    Serial.print("module: ");
    Serial.println(rmodule);
    Serial.print("state: ");
    Serial.println(rstate);
    Serial.print("latitude: ");
    Serial.println(rslat);
    Serial.print("longitude: ");
    Serial.println(rslng);
    Serial.print("height: ");
    Serial.println(rshgt);
    Serial.print("heading: ");
    Serial.println(rsheading);
    
  }

  delay(100);
}


void mergeData(String module_, String state_, String slat_, String slng_, String shgt_, String sheading_){


  rfData = module_ +"," +state_ +"," + slat_ +"," + slng_ +"," + shgt_ +"," + sheading_;
  sdData = module_ +"\t"+state_ +"\t"+ slat_ +"\t"+ slng_ +"\t"+ shgt_ +"\t"+ sheading_;
}

int readPck(String pck_){

  char comma = ',';

  int i = pck_.indexOf(comma);
  rmodule = pck_.substring(0,i);

  int j = pck_.indexOf(comma,i+1);
  rstate = pck_.substring(i+1,j);

  i = pck_.indexOf(comma,j+1);
  rslat = pck_.substring(j+1,i);

  j= pck_.indexOf(comma,i+1);
  rslng = pck_.substring(i+1,j);

  i = pck_.indexOf(comma,j+1);
  rshgt = pck_.substring(j+1,i);

  j= pck_.indexOf(comma,i+1);
  rsheading = pck_.substring(i+1,j);

  char buf0[rslat.length()];
  char buf1[rslng.length()];
  char buf2[rshgt.length()];
  char buf3[rsheading.length()];

  if(rslat.length() != 0)
  {
  	rslat.toCharArray(buf0,rslat.length());
  	rlat = atof(buf0); 
    rslng.toCharArray(buf1,rslng.length());
    rlng = atof(buf1); 
    rshgt.toCharArray(buf2,rshgt.length());
    rhgt = atof(buf2);
    rsheading.toCharArray(buf2,rsheading.length());
    rheading = atof(buf3);

    return 0;
  }

  else{
    rlat = 0;
    rlng = 0;
    rhgt = 0;
    return -1;
  }
}

void compassRenew(){
  Wire.beginTransmission(SlaveAddress);
  Wire.write(ReadAddress);              // The "Get Data" command
  Wire.endTransmission();

  //time delays required by HMC6352 upon receipt of the command
  //Get Data. Compensate and Calculate New Heading : 6ms
  delay(6);

  Wire.requestFrom(SlaveAddress, 2); //get the two data bytes, MSB and LSB

  //"The heading output data will be the value in tenths of degrees
  //from zero to 3599 and provided in binary format over the two bytes."
  byte MSB = Wire.read();
  byte LSB = Wire.read();

  headingSum = (MSB << 8) + LSB; //(MSB / LSB sum)
  headingInt = headingSum / 10;
  sHeading = dtostrf(headingInt, 1, 1, Buffer);

  if(sHeading != "")
  {
   // Serial.println(sHeading);
  }
}


void sdWrite(String dataString){
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

   // if the file is available, write to it:
   if(dataFile){
    dataFile.println(dataString);
    dataFile.close();

      // print to the serial port too:
      Serial.println(dataString);
    }
}
