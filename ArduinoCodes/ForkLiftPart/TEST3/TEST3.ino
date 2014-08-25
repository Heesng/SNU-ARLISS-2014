#include <HerkuleX.h>

#define RX   0        // Connected with HerkuleX TX Pin
#define TX   1        // Connected with HerkuleX RX Pin
#define MOTORID  219  // HerkuleX Servo Motor ID

void setup()  
{
  // Open SoftwareSerial with 57600 baudrate
  // To set up HerkuleX servo baudrate, See. the library manual
  HerkuleX.begin(57600, RX, TX);  
  delay(10);
  // Torque ON
  HerkuleX.torqueOn(MOTORID);
  
  if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
    HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
  }
  
  HerkuleX.moveAngle(MOTORID, 160, 1000, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);
  delay(1000);

}


void loop() // run over and over
{
       // Move HerkuleX to 0 degree by 112ms, Turn green LED on.
      HerkuleX.moveAngle(MOTORID, 160, 1000, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);  

}


