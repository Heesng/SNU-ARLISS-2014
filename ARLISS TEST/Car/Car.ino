#include <GPS.h>
#include <Wire.h>
#include <LSM303.h>
#include <Math.h>
#include <Servo.h>
#include <RF.h>
#include <SD.h>
#include <DCS.h>

/*
Car functions
*/
void vel(int a);
void steer(float a,float b,float c,float d,float e);
void go(float a,float b,float c,float d,float e, float f, float g);
void sdWrite(String dataString);
float distance(GPS gps_, DCS dcs_);

/*
Car Variables
*/
DCS dcs;
GPS gps;
RF rf;
Servo Carsteer;
Servo Carspeed;
LSM303 compass;

float destlat = 37.275371;
float destlong = 126.569137;
float steer_car,dsteer_car,isteer_car = 0, steer_car0;
float Pgain = 3, Dgain, Igain;
const String module = "R";
const int chipSelect = 9; //SD card chip select
char x [6];

int d,g=0, r=0; //dcs,gps,rfrcv
int D=0; //, Docking
float dist=1000;
String state = "";
String rcvPck = "";

void setup(){
  state = "S";
  Serial.begin(9600);
  Serial1.begin(1200);
  Serial2.begin(9600);
  //for compass sensor
  Wire.begin();
  compass.init();
  compass.enableDefault();  
  compass.m_min = (LSM303::vector<int16_t>){
    -703,-737, -746  };
  compass.m_max = (LSM303::vector<int16_t>){
    +695, +461, +635  };
  //for steer
  Carsteer.attach(2);
  //for speed
  Carspeed.attach(3);
  delay(2000);
  Carspeed.write(10);
  delay(2000);
}

void loop(){
  g = gps.renew();
  //Serial.println("a");
  float lat = gps.getLat()/100;
  float lng = gps.getLng()/100;
  //double satheading = 0;//rf.getheading();
  //for compass sensor
  //Serial.println("b");
  compass.read();
  //Serial.println("c");
  float heading_ = compass.heading();
  dtostrf(heading_,3,1,x);
  //Serial.println("d");
  
  if(g==1){
  dcs.mergeData(module,state,gps.getSLat(),gps.getSLng(),gps.getSHgt(),x);
  //sdWrite(dcs.getSdData());
  rf.sendPck(dcs.getRfData());
  }
  r = rf.receivePck(rcvPck);
  if(r==0){
  dcs.readPck(rcvPck);
  dist = distance(gps,dcs);
  }
  
  /*
use dcs functions dcs.function();

 	String getSLat();
	String getSLng();
	String getSHgt();
	String getSHeading();
	double getLat();
	double getLng();
	double getHgt();
	double getHeading();

*/

  go(dcs.getLat(),dcs.getLng(),lat,lng, heading_, dcs.getHeading());
  //Serial.println(lat,6);
  //Serial.println(lng,6);
//  steer(destlat,destlong,lat,lng,heading_);
  //Serial.println("e");
}

void go(float destlat,float destlong,float lat,float lng,float heading_, float sheading_){
  float semidestlat = destlat - 0.0009*sin(sheading_);
  float semidestlong = destlong - 0.0009*cos(sheading_);
  
  if(((destlat - lat)*(destlat - lat) + (destlong - lng)*(destlong - lng)) < ((semidestlat - lat)*(semidestlat - lat) + (semidestlong - lng)*(semidestlong - lng))){
    semidestlat = destlat + 0.0009*sin(sheading_);
    semidestlong = destlong + 0.0009*cos(sheading_);
  }
  
  if((destlat-lat)*(destlat-lat) + (destlong-lng)*(destlong-lng)> 0.0009*0.0009){
    vel(60);
    steer(semidestlat,semidestlong,lat,lng,heading_);
  }
  else{
    vel(30);
    steer(destlat,destlong,lat,lng, heading_);
  }
}

void steer(float destlat,float destlong,float flatitude,float flongitude, float heading_){
  float dy = destlat - flatitude;
  float dx = cos(flatitude*3.141592/180)*(destlong-flongitude);
  float angle = atan(dy/dx)*180/3.141592;
  //float dsx = cos(satheading*3.14/180);
  //float dsy = sin(satheading*3.14/180);
  //int power = 0;//1/((destlat-flatitude)*(destlat-flatitude)+(destlong-flongitude)*(destlong-flongitude));
  // float angle = atan((dy+power*dsy)/(dx+power*dsx))*180/3.141592;
  if((dy>0)&&(dx>0)){
    angle = 90 - angle;
  }
  else if((dy>0)&&(dx<0)){
    angle = 270 - angle;
  }
  else if((dy<0)&&(dx>0)){
    angle = 90 - angle;
  }
  else if((dy<0)&&(dx<0)){
    angle = 270 - angle;
  }
  
  Serial.println(angle);
  
  steer_car = angle - heading_;
  
  Serial.println(steer_car);  
  
  float psteer_car = steer_car;
  if(psteer_car<0){
    psteer_car = psteer_car + 360;
  }
  
  if(180 < psteer_car && psteer_car <= 360){
    psteer_car = psteer_car - 360;
  }
  
  dsteer_car = psteer_car - steer_car0;
  steer_car0 = psteer_car;
  isteer_car = isteer_car + dsteer_car;
  
  float osteer_car = Pgain*psteer_car + Dgain*dsteer_car + Igain*isteer_car;
  float osteer = 0;

  if (-180.0 <= psteer_car && psteer_car < -90.0){
    osteer = 120;
  }
  else if(-90.0 <= psteer_car && psteer_car <= 90.0){
    //osteer = map(psteer_car, 270, 360, 120, 90);// (-1)*(3*steer)/9 + 210;
    //osteer = 180 - 90*pow( 2, osteer_car);
    if(osteer_car< -90){
      osteer_car = -90;
    }
    else if(osteer_car>90){
      osteer_car = 90;
    }
    osteer = map(osteer_car, -90, 90, 120, 60);
  }
  else if(90.0 <= psteer_car && psteer_car < 180.0){ 
    osteer = 60;
  }
  Carsteer.write(osteer);
  Serial.println(osteer);
}

void vel(int velocity){
  unsigned long settime = millis();
  unsigned long currenttime = millis();
  unsigned long ftime = settime + velocity;
  unsigned long stime = settime + velocity + (100-velocity)/3;
  unsigned long ttime = settime + velocity + 2*(100-velocity)/3;
  unsigned long etime = settime + 100;
  while(currenttime<ftime){
    Carspeed.write(110);
    currenttime = millis();
  }
  while(currenttime<stime){
    Carspeed.write(100);
    currenttime = millis();
  }
  while(currenttime<ttime){
    Carspeed.write(95);
    currenttime = millis();
  }
  while(currenttime<etime){
    Carspeed.write(90);
    currenttime = millis();
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

float distance(GPS gps_, DCS dcs_){
	float lat1 = gps_.getLat();
	float lng1 = gps_.getLng();

	float lat2 = dcs_.getLat();
	float lng2 = dcs_.getLng();

	float latL, lngL, dist;
	latL = 2*3.14159*6378000*(lat1-lat2)/100/180;
	lngL = 2*3.14159*6378000*(lat1-lat2)/100/180;

	dist = sqrt(latL*latL+lngL*lngL);

	return dist;
}
