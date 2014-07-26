/*
  Dongbu Robot HerkuleX Servo Motor Example
  
  Author : JaeGon - 2012.11.08
  */

  #include <HerkuleX.h>

#define RX   8        // Connected with HerkuleX TX Pin
#define TX   9        // Connected with HerkuleX RX Pin
#define MOTORID  253  // HerkuleX Servo Motor ID

void setup()  
{
  Serial.begin(9600);    // Open serial communications
  // Open SoftwareSerial with 57600 baudrate
  // To set up HerkuleX servo baudrate, See. the library manual
  HerkuleX.begin(57600, RX, TX);  
  delay(10);
  // Torque ON
  HerkuleX.torqueOn(MOTORID);
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
    
    if (incomingbyte == '1') {
      Serial.println("Move to 0 degree");
      // Move HerkuleX to -30 degree by 112ms, Turn blue LED on.
      HerkuleX.moveAngle(MOTORID, 0, 2000, HERKULEX_LED_RED);  
    }
    
    if (incomingbyte == 'a') {
      Serial.println("Current angle");
      // Get current HerkuleX angle and send it to PC
      Serial.println(HerkuleX.getAngle(MOTORID));  
    }
    
    if (incomingbyte == '2') {
      Serial.println("Move to 90 degree");
      // Move HerkuleX to 100 degree by 112ms, Turn blue LED on.
      HerkuleX.moveAngle(MOTORID, 90, 2000, HERKULEX_LED_BLUE);  
    }

    if (incomingbyte == '3') {
      Serial.println("Move to 90 degree");
      // Move HerkuleX to 100 degree by 112ms, Turn blue LED on.
      HerkuleX.moveAngle(MOTORID, 110, 1000, HERKULEX_LED_GREEN);  
    }
    
    if (incomingbyte == 't') {
      Serial.println("Turn Speed");
      // Get current turn speed and send it to PC
      Serial.println(HerkuleX.getTurnSpeed(MOTORID));
    }
    
    if (incomingbyte == 'q') {
      Serial.println("Finish");
      // Torque OFF
      HerkuleX.torqueOff(MOTORID);
    }
  }
}


