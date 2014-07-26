#include <GPS.h>
#include <Wire.h>
#include <LSM303.h>
#include <Math.h>
#include <Servo.h>
#include<PID_v1.h>

/*
Car functions
*/
void vel(int a);
void steer(float a,float b,float c,float d,float e);
void go(float a,float b,float c,float d,float e);

/*
Car Variables
*/
GPS gpsEx;
Servo Carsteer;
Servo Carspeed;
LSM303 compass;
float destlat = 37.275398;
float destlong = 126.569114;
float steer_car;
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint,1,0,0, DIRECT);

void setup(){
  Serial.begin(9600);
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

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop(){
  gpsEx.renewGPS();
  Serial.println("a");
  float lat = gpsEx.getLat()/100;
  float lng = gpsEx.getLng()/100;
  //double satheading = 0;//rf.getheading();
  //for compass sensor
  Serial.println("b");
  compass.read();
  Serial.println("c");
  float heading_ = compass.heading();
  Serial.println("d");
  go(destlat,destlong,lat,lng, heading_);
//  Serial.println(lng,6);
//  steer(destlat,destlong,lat,lng,heading_);
  Serial.println("e");
}

void go(float destlat,float destlong,float lat,float lng,float heading_){
    if((destlat-lat)*(destlat-lat) + (destlong-lng)*(destlong-lng)> 0.00026*0.00026){
      vel(60);
      steer(destlat,destlong,lat,lng,heading_);
    }
    else{
      vel(0);
    }
}

void steer(float destlat,float destlong,float flatitude,float flongitude,float heading_){
  float dy = destlat - flatitude;
  float dx = cos(flatitude*3.141592/180)*(destlong-flongitude);
  float angle = atan(dy/dx)*180/3.141592;
  //float dsx = cos(satheading*3.14/180);
  //float dsy = sin(satheading*3.14/180);
  //int power = 0;//1/((destlat-flatitude)*(destlat-flatitude)+(destlong-flongitude)*(destlong-flongitude));
  // float angle = atan((dy)/(dx))*180/3.141592;
  
  //initialize the variables we're linked to
  Input = heading_;
  Setpoint = angle;
  myPID.Compute();
  steer_car = Output;

//  float steer_car = angle - heading_;
  if  (steer_car<0){
    steer_car = 5;
  }
  else if  (steer_car>180){
    steer_car = 175;
  }
  Serial.println(heading_);
  Serial.println(Output);
  Serial.println(steer_car);
  float osteer=0;
//  if (180.0 <= steer_car && steer_car < 270.0){
//    osteer = 60;
//  }
//  else if(270.0 <= steer_car && steer_car <= 360.0){
//    osteer = (3*steer_car)/9 - 30;
//  }
//  else if(0.0 <= steer_car && steer_car < 90.0){
//    osteer = (3*steer_car)/9 + 90;
//  }
//  else if(90.0 <=steer_car && steer_car < 180.0){
//    osteer = 120;
//  }
  if (180.0 <= steer_car && steer_car < 270.0){
    osteer = 120;
  }
  else if(270.0 <= steer_car && steer_car <= 360.0){
    osteer = map(steer_car, 270, 360, 120, 90);// (-1)*(3*steer_car)/9 + 210;
  }
  else if(0.0 <= steer_car && steer_car < 90.0){
    osteer = map(steer_car, 0, 90, 90, 60);//(-1)*(3*steer_car)/9 + 90;
  }
  else if(90.0 <=steer_car && steer_car < 180.0){
    osteer = 60;
  }
  Carsteer.write(osteer);
}

void vel(int velocity){
//  for(int i = 0; i<10;i++){
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
//  }
}
