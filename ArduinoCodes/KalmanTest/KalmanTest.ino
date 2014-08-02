#include<KalmanLatLong.h>
#include<GPS.h>
GPS gps;
KalmanLatLong kalman(100);
float lat=3727.5390;
float lng=12656.8574;
int i = 0;


 void setup(){
   kalman.SetState(0,0,10000,1000);
   Serial.begin(9600);
   Serial2.begin(9600);
 }
 void loop(){
   
   i = gps.renewGPS();
   lat = gps.getLat();
   lng = gps.getLng();
   Serial.print("raw data:  ");
   Serial.print(lat,4);
   Serial.print("  ");
   Serial.println(lng,4);
   if(i==0 && lat!=0){
     kalman.Process(lat,lng,1,1000);
     Serial.print("filtered data:  ");
     Serial.print(kalman.get_lat(),4);
     Serial.print("  "); 
     Serial.println(kalman.get_lng(),4);
   }
   delay(500);

 }
