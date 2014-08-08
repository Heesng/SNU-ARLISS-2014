#include <HerkuleX.h>

#define RX   10        // Connected with HerkuleX TX Pin
#define TX   11        // Connected with HerkuleX RX Pin
#define MOTORID  219  // HerkuleX Servo Motor ID

void setup()  
{
  Serial.begin(9600);    // Open serial communications
  // Open SoftwareSerial with 57600 baudrate
  // To set up HerkuleX servo baudrate, See. the library manual
  HerkuleX.begin(57600, RX, TX);  
  delay(10);
  // Torque ON
  HerkuleX.torqueOn(MOTORID);
  
  if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
    HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
  }
  
  HerkuleX.moveAngle(MOTORID, 0, 10, HERKULEX_LED_GREEN | HERKULEX_LED_BLUE | HERKULEX_LED_RED);
  delay(1000);

}


void loop() // run over and over
{
  unsigned char incomingbyte = 0;
  
  // Check current HerkuleX status
  if (HerkuleX.getStatus(MOTORID) != HERKULEX_STATUS_OK) {
    HerkuleX.clear(MOTORID);  // If there is an error dectected, clear it
  }

  if (Serial.available() > 0) {  // If Serial(with PC) is available
    incomingbyte = Serial.read();  // Reading a byte from PC

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


