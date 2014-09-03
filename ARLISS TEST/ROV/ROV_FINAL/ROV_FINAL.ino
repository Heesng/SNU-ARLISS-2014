#include <GPS.h>
#include <Wire.h>
#include <LSM303.h>
#include <Math.h>
#include <Servo.h>
#include <SD.h>
#include <RF.h>
#include <CTS.h>
#include <HerkuleX.h>

#define RX 0 
#define TX 1
#define MOTORID 219
/*
Car functions
*/
void steer(float a,float b,float c,float d,float e);
void go(float a,float b,float c,float d,float e);
void rendezvous();
void insert(float a);

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
CTS CTS1;
Servo Camera;
Servo Carsteer;
LSM303 compass;
int goback = 1;
float startlat = 37.276210;
float startlong = 126.570167;
float lat;
float lng;
float steer_car,dsteer_car,isteer_car = 0, steer_car0;
float Pgain = 1, Dgain, Igain;
float osteer;
int motor = 2;
int r=0,g,d;
float rsangle = 0;
int val = 0;
int data=0;
int center=120;
int sonar1 = 49;
int sonardist1 = 100;
int offset = 11;

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
  delay(3000);
  state = "S";
  //Serial.begin(9600);
  HerkuleX.begin(57600, RX, TX);
  delay(10);  
  Serial1.begin(1200);
  Serial2.begin(9600);
  
  /*
  Herkulex setting
  */  
  HerkuleX.torqueOn(MOTORID);
  if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
    HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
  }
  HerkuleX.moveAngle(MOTORID, -30, 100, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);
  delay(1000);

  /*
  SDcard setting
  */
  pinMode(10, OUTPUT);
//  if (!SD.begin(chipSelect)) {
//    return;
//  } 

  /*
  Compass setting
  */
  Wire.begin();
  compass.init();
  compass.enableDefault();  
  compass.m_min = (LSM303::vector<int16_t>){
      -525,   -484,   -381  };
    compass.m_max = (LSM303::vector<int16_t>){
       +461,   +409,   +359 };

  /*
  Driving setting
  */
  //for cameraservo
  Camera.attach(4);
  //for steer
  Carsteer.attach(3);
  //for speed
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
  analogWrite(motor, 181);
  delay(1000);
  
  //for sonar
  pinMode(sonar1, INPUT);
}

void loop(){
//  String dataString = "";
  //while(gpsEx.getLat()==0){
    g = gpsEx.renew();
  //}
//  Serial.println("a");
  
    lat = gpsEx.getLat()/100;
    lng = gpsEx.getLng()/100;
  
//  dataString += String((long)(lat*1000000));
//  dataString += "	";
//  dataString += String((long)(lng*1000000));
  //double satheading = 0;//rf.getheading();
  //for compass sensor
//  Serial.println("b");
  compass.read();
//  Serial.println("c");
  float heading_ = compass.heading((LSM303::vector<int>){0,1,0});
  sHeading = dtostrf(heading_,1,1,Buffer);
//  dataString += "	";
//  dataString += String((int)(heading_*10));
//  Serial.println("d");
  //float sheading_ = 0;
  go(rlat,rlng,lat,lng, heading_);
//  dataString += "	";
//  dataString += String((int)osteer);
//  Serial.println(lat,6);
//  Serial.println(lng,6);
//  Serial.println(heading_);
  //steer(destlat,destlong,lat,lng,heading_);
//  Serial.println("e");
//  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
//  if (dataFile) {
//    dataFile.println(dataString);
//    dataFile.close();
    // print to the serial port too:
//  }
  /*
  RF sending code
  
  if(g==1){
    mergeData(module,state,gpsEx.getSLat(),gpsEx.getSLng(),gpsEx.getSHgt(),sHeading);
    rf.sendPck(rfData);
  }
*/
//  Serial.println("f");
  
  r = rf.receivePck(rcvPck);
  //Serial.println(rcvPck);
  if(r==0){
    readPck(rcvPck);
    //Serial.print("module: ");
    //Serial.println(rmodule);
    //Serial.print("state: ");
    //Serial.println(rstate);
    //Serial.print("latitude: ");
    //Serial.println(rslat);
    //Serial.print("longitude: ");
    //Serial.println(rslng);
    //Serial.print("height: ");
    //Serial.println(rshgt);
    //Serial.print("heading: ");
    //Serial.println(rsheading);
  }  // if the file isn't open, pop up an error:
//  else {
//    Serial.println("error opening datalog.txt");
//  }
//  Serial.println("h");
}
//motor neutral = 179~184
//motor possible range = 
void go(float destlat,float destlong,float lat,float lng,float heading_){
  if(((destlat-lat)*(destlat-lat) + (destlong-lng)*(destlong-lng) > 0.00009*0.00009)&&goback){
    analogWrite(motor, 186);
    steer(destlat,destlong,lat,lng,heading_);
    state = "D";
  }
  else if (goback){
    analogWrite(motor, 181);
    goback = 0;
    state = "M";
    //steer(destlat,destlong,lat,lng, heading_);
  }
  if(((startlat-lat)*(startlat-lat) + (startlong-lng)*(startlong-lng) > 0.00009*0.00009)&&goback==0){
    analogWrite(motor, 186);
    steer(startlat,startlong,lat,lng,heading_);
    state = "R";
  } 
  else if (goback == 0){
    analogWrite(motor, 181);
  }
}

void steer(float destlat,float destlong,float flatitude,float flongitude, float heading_){
  float dy = destlat - flatitude;
  float dx = cos(flatitude*3.141592/180)*(destlong-flongitude);
  float angle = atan(dy/dx)*180/3.141592;
  float offset = 11;
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
  else{
    angle = 270 - angle;
  }
  
  //Serial.println(angle);
  
  steer_car = angle - heading_;
  //steer_car = steer_car + 40;
  //Serial.println(steer_car);  
  
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
  osteer_car = osteer_car - offset;
  
  if (-180.0 <= psteer_car && psteer_car < -90.0){
    osteer = 120-offset;
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
    osteer = map(osteer_car, -90, 90, 120-offset, 60-offset);
  }
  else if(90.0 <= psteer_car && psteer_car < 180.0){ 
    osteer = 60-offset;
  }
  Carsteer.write(osteer);
  //osteer = 79; - > offset정중
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
    rlat = atof(buf0)/100; 
    rslng.toCharArray(buf1,rslng.length());
    rlng = atof(buf1)/100; 
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
void rendezvous(){
  int i = 0;
  Camera.write(90);
  delay(100);
  val = CTS1.location();
  if(val == 200){
    for(i = 0;i<181;i++){
      Camera.write(i);
      val = CTS1.location();
      delay(100); 
      if(val != 200){
        break;
      }
    }
    if(0<=i&&i<90){
      Carsteer.write(110-offset);
      delay(100); 
      analogWrite(motor, 175);    
      delay(300);
      Carsteer.write(70-offset);
      delay(100);
      analogWrite(motor, 175);
      delay(300);
      Carsteer.write(90-offset);
      delay(100);
    }
    else if(90<=i&&i<=180){
      Carsteer.write(70-offset);
      delay(100); 
      analogWrite(motor, 175);    
      delay(300);
      Carsteer.write(110-offset);
      delay(100);
      analogWrite(motor, 175);
      delay(300);
      Carsteer.write(90-offset);
      delay(100);
    }
    else{
      Carsteer.write(90-offset);
      delay(100);
      analogWrite(motor, 186);
      delay(100);
    }    
  }
  else{
    if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
      HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
    }
    HerkuleX.moveAngle(MOTORID, 150, 100, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);
    delay(1000);
    while(sonardist1>5){
      Camera.write(90);
      val = CTS1.location();
      insert(val);
      sonardist1 = gpsEx.liftsonar(sonar1);
    }
    if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
      HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
    }
    HerkuleX.moveAngle(MOTORID, -30, 100, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);
    delay(1000);
    sonardist1 = 100;
  }
}

void insert(float rsangle){
  if(-40<rsangle&&rsangle<40){
    Carsteer.write(90-offset);
    delay(100);
    analogWrite(motor, 186);
    delay(100);
  }
  else if(40<rsangle&&rsangle<120){
    Carsteer.write(70-offset);
    delay(100);
    analogWrite(motor, 175);
    delay(300);
    Carsteer.write(110-offset);
    delay(100); 
    analogWrite(motor, 175);
    delay(300);
    Carsteer.write(90-offset);
    delay(100);
    analogWrite(motor, 186);
    delay(400);
  }
  else if(-120<rsangle&&rsangle<-40){
    Carsteer.write(110-offset);
    delay(100); 
    analogWrite(motor, 175);    
    delay(300);
    Carsteer.write(70-offset);
    delay(100);
    analogWrite(motor, 175);
    delay(300);
    Carsteer.write(90-offset);
    delay(100);
    analogWrite(motor, 186);
    delay(400);    
  }
  else if (rsangle == 200){
    Carsteer.write(90-offset);
    delay(100);
    analogWrite(motor, 181);
    delay(100);
  }
}
  
