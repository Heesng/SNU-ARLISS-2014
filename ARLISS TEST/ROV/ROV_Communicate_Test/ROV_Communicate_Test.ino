#include <GPS.h>
#include <Wire.h>
#include <LSM303.h>
#include <Math.h>
#include <Servo.h>
#include <SD.h>
#include <RF.h>

/*
Car functions
*/
void steer(float a,float b,float c,float d,float e);
void go(float a,float b,float c,float d,float e);


/*
Data process functioncs
*/
void mergeData(String module_, String state_, String slat_, String slng_, String shgt_, String sheading_);
int readPck(String pck_);

/*
Car Variables
*/
RF rf;
GPS gpsEx;
Servo Carsteer;
LSM303 compass;
float destlat = 37.279319;
float destlong = 126.570091;
float steer_car,dsteer_car,isteer_car = 0, steer_car0;
float Pgain = 1, Dgain, Igain;
float osteer;
int motor = 2;
int r=0,g,d;

String sHeading;
char Buffer[5];


String state = "";
String rcvPck = "";

String rfData = "";
String sdData = "";

String rmodule;//Received module
String rstate;//Received state
String rslat;//Received String latitude
String rslng;//Received String longitude
String rshgt;//Received String height
String rsheading;//Received String heading

float rlat = 0;//Received latitude
float rlng = 0;//Received longitude
float rhgt = 0;//Received height
float rheading = 0;//Received heading

String module = "R";
int chipSelect = 53;

void setup(){
  state = "S";
  Serial.begin(9600);
  Serial1.begin(1200);
  Serial2.begin(9600);

  /*
  SDcard setting
  */
  pinMode(10, OUTPUT);
  

  /*
  Compass setting
  */
  Wire.begin();
  compass.init();
  compass.enableDefault();  
  compass.m_min = (LSM303::vector<int16_t>){
    -703,-737, -746  };
    compass.m_max = (LSM303::vector<int16_t>){
      +695, +461, +635  };
  Serial.println("Let's begin");
  
}

void loop(){
  
  //double satheading = 0;//rf.getheading();
  //for compass sensor
  compass.read();
  float heading_ = compass.heading((LSM303::vector<int>){0,1,0});
  sHeading = dtostrf(heading_,1,1,Buffer);

  r = rf.receivePck(rcvPck);
  Serial.print(r);
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
  }  // if the file isn't open, pop up an error:
//  else {
//    Serial.println("error opening datalog.txt");
//  }
}
//motor neutral = 186~188
//motor possible range = 
void go(float destlat,float destlong,float lat,float lng,float heading_){
  if((destlat-lat)*(destlat-lat) + (destlong-lng)*(destlong-lng) > 0.00009*0.00009){
    analogWrite(motor, 190);
    steer(destlat,destlong,lat,lng,heading_);
  }
  else{
    analogWrite(motor, 187);
    //steer(destlat,destlong,lat,lng, heading_);
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
  //delay(1000);
  //Serial.println(osteer);
  //dataString += String((int)osteer);
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
