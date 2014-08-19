#include <HerkuleX.h>
#include <GPS.h>
#include <Wire.h>
#include <LSM303.h>
#include <Math.h>
#include <Servo.h>
#include <SD.h>

#define RX   0        // Connected with HerkuleX TX Pin
#define TX   1        // Connected with HerkuleX RX Pin
#define MOTORID  219  // HerkuleX Servo Motor ID

/*
Car functions
*/
void steer(float a,float b,float c,float d,float e);
void go(float a,float b,float c,float d,float e,float f);
void insert(float a);
void lift();
/*
Car Variables
*/
GPS gpsEx;
Servo Carsteer;
LSM303 compass;
float destlat = 37.279319;
float destlong = 126.570091;
float steer_car,dsteer_car,isteer_car = 0, steer_car0;
float Pgain = 1, Dgain, Igain;
float osteer;
int tactswitch = 0;
int motor = 2;
int sonar1 = 22;
int sonar2 = 23;

const int chipSelect = 53;

void setup(){
  Serial.begin(9600);
  HerkuleX.begin(57600, RX, TX);  
  delay(10);
  // Torque ON
  HerkuleX.torqueOn(MOTORID);
  if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
    HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
  }
  HerkuleX.moveAngle(MOTORID, 0, 10, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);
  delay(1000);
  
  Serial2.begin(9600);
  //for compass sensor
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    return;
  } 
  Wire.begin();
  compass.init();
  compass.enableDefault();  
  compass.m_min = (LSM303::vector<int16_t>){
    -703,-737, -746  };
  compass.m_max = (LSM303::vector<int16_t>){
    +695, +461, +635  };
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
  pinMode(sonar1,INPUT); // 센서 Echo 핀
  pinMode(sonar2,INPUT); // 센서 Echo 핀
}

void loop(){
  if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
    HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
  }
  
  float sheading_ = 0;
  String dataString = "";
  //while(gpsEx.getLat()==0){
    gpsEx.renew();
  //}
  //Serial.println("a");
  float lat = gpsEx.getLat()/100;
  float lng = gpsEx.getLng()/100;
  dataString += String((long)(lat*1000000));
  dataString += "	";
  dataString += String((long)(lng*1000000));
  //double satheading = 0;//rf.getheading();
  //for compass sensor
  //Serial.println("b");
  compass.read();
  //Serial.println("c");
  float heading_ = compass.heading((LSM303::vector<int>){0,1,0});
  dataString += "	";
  dataString += String((int)(heading_*10));
  //Serial.println("d");
  //float sheading_ = 0;

  dataString += "	";
  dataString += String((int)osteer);
  Serial.println(lat,6);
  Serial.println(lng,6);
  //steer(destlat,destlong,lat,lng,heading_);
  //Serial.println("e");
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
  }  
  // if the file isn't open, pop up an error:
//  else {
//    Serial.println("error opening datalog.txt");
//  }
  go(destlat,destlong,lat,lng, heading_, sheading_);
}
//motor neutral = 186~188
//motor possible range = 

void go(float destlat,float destlong,float lat,float lng,float heading_, float sheading_){
  float semidestlat = destlat - 0.0009*cos(sheading_);
  float semidestlong = destlong - 0.0009*sin(sheading_); 
  float rsangle;
  
  if(((destlat - lat)*(destlat - lat) + (destlong - lng)*(destlong - lng)) < ((semidestlat - lat)*(semidestlat - lat) + (semidestlong - lng)*(semidestlong - lng))){
    semidestlat = destlat + 0.0009*cos(sheading_);
    semidestlong = destlong + 0.0009*sin(sheading_);
  } 
  
  if((destlat-lat)*(destlat-lat) + (destlong-lng)*(destlong-lng) >= 0.0009*0.0009){
    analogWrite(motor, 190);
    steer(semidestlat,semidestlong,lat,lng,heading_);
  }
  else if(((destlat-lat)*(destlat-lat) + (destlong-lng)*(destlong-lng) < 0.0009*0.0009) && ((destlat-lat)*(destlat-lat) + (destlong-lng)*(destlong-lng) > 0.00009*0.00009)){
    analogWrite(motor, 190);
    steer(destlat,destlong,lat,lng,heading_);
  }
  else{
    while(gpsEx.liftsonar(sonar1)>3){
      Carsteer.write(90);
      analogWrite(motor, 186);
      insert(rsangle);
    }
    lift();
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
//  delay(1000);
 // Serial.println(osteer);
  //dataString += String((int)osteer);
}

void insert(float rsangle){
  if(-10<rsangle&&rsangle<10){
    analogWrite(motor, 200);
    delay(2000);
  }
  else if(-120<rsangle&&rsangle<-10){
    Carsteer.write(60);
    analogWrite(motor, 170);
    delay(1000);
    Carsteer.write(120);
    analogWrite(motor, 170);
    delay(1000);
  }
  else if(10<rsangle&&rsangle<120){
    Carsteer.write(120);
    analogWrite(motor, 170);    
    delay(1000);
    Carsteer.write(60);
    analogWrite(motor, 170);
    delay(1000);    
  }
}

void lift(){
  if (Serial.available() > 0) {  // If Serial(with PC) is available
    unsigned char incomingbyte = Serial.read();  // Reading a byte from PC

    if (incomingbyte == '0') {
      Serial.println("Move angle to 0");
      // Move HerkuleX to 0 degree by 112ms, Turn green LED on.
      HerkuleX.moveAngle(MOTORID, 0, 100, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);  
    }

    if (incomingbyte == '1') {
      Serial.println("Move angle to 170");
      // Move HerkuleX to 170 degree by 112ms, Turn blue LED on.
      HerkuleX.moveAngle(MOTORID, 160, 100, HERKULEX_LED_BLUE);  
    }
    
    if (incomingbyte == '2') {
      Serial.println("Move angle to -170");
      // Move HerkuleX 512, 11.2ms * 100 = 1120ms, Turn red LED on
      HerkuleX.moveAngle(MOTORID, -160, 100, HERKULEX_LED_GREEN);
    }

    if (incomingbyte == 'a') {
      Serial.println("Current angle");
      // Get current HerkuleX angle and send it to PC
      Serial.println(HerkuleX.getAngle(MOTORID));  
    }
    
    if (incomingbyte == 'q') {
      Serial.println("Finish");
      // Torque OFF
      HerkuleX.torqueOff(MOTORID);
    }
  }
}
