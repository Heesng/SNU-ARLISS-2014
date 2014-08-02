/*
  HerkuleX.h - Dongbu Robot HerkuleX Servo library for Arduino - Version 0.1
  Copyright (c) 2012 Dongbu Robot. All right reserved.
  Author : JaeGon, Ahn, 2012-11-08
  E-mail : jgahn@dongbu.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  HerkuleX Servo Manual (http://www.hovis.co.kr/guide/herkulexeng.pdf)
  ===================================================================
  HerkuleX Servo Protocol(To control HerkuleX Servos  See. Manual p18)

  Header  Pkt Size     ID        CMD     Chksum1  ChkSum2   Data[n]
  0xFF 0xFF 0x07~0xDF 0x00~0xFE 0x01~0x09     Refer to Manual p20.
  2bytes   1byte      1byte     1byte     1byte    1byte   ~216bytes
  ====================================================================

  ====================================================================
  HerkuleX Pin Description See. Manual p17)

	-----------
	-         -
	-4321 4321-
	-         -
	-         -
	-----------

	1: GND
	2: VCC (Input voltage must be in 7~12VDC, Optimized 7.4V)
	3: TX  (This pin has to be connected with Arduino RX pin)
	4: RX  (This pin has to be connected with Arduino TX pin)
  ====================================================================

  ====================================================================
  NOTICE!

  This library has been only tested on Arduino UNO with badurate 57600.
  If you control HerkuleX Servos through SoftwareSerial library,
  We highly recommend you to set up your servo baudrate as 57600 like

	//HerkuleX.begin(57600)

  The default setting of HerkuleX Servo baudrate is 115200 (actually 117647)
  Setting up your HerkuleX servo baudrate by using HekruleX Manager

	Download URL:
	http://www.dongburobot.com/jsp/cms/view.jsp?code=100703&isSkin=Y&cmd=view&boardCode=100171&bseq=5222

	How To:
	1. Run HerkuleX Manager
	2. Conneting your servo
	3. Setting > Other set-up > User Classfication > Advance
	4. Detailed set-up
	3. EEP Register address 4 Baudrate, Set the register as 0x22
	See. Manual p26.

  If you are using other Arduino board like Mega, Due and etc.,
  you might modify some piece of codes to correctly work following functions
	- getStatus()
	- getTurnSpeed()
	- getPos()
	- getAngle()

  ====================================================================
*/

#ifndef _HERKULEX_H
#define _HERKULEX_H

#if defined(ARDUINO) && ARDUINO >= 100  // Arduino IDE Version
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define BUFFER_SIZE 30

// HerkuleX LED - See. Manual p48.
static byte HERKULEX_LED_RED 		= 0x10;
static byte HERKULEX_LED_GREEN		= 0x04;
static byte HERKULEX_LED_BLUE 		= 0x08;

// HerkuleX Status Error - See. Manual p39.
static byte HERKULEX_STATUS_OK					= 0x00;
static byte HERKULEX_ERROR_INPUT_VOLTAGE 		= 0x01;
static byte HERKULEX_ERROR_POS_LIMIT			= 0x02;
static byte HERKULEX_ERROR_TEMPERATURE_LIMIT	= 0x04;
static byte HERKULEX_ERROR_INVALID_PKT			= 0x08;
static byte HERKULEX_ERROR_OVERLOAD				= 0x10;
static byte HERKULEX_ERROR_DRIVER_FAULT  		= 0x20;
static byte HERKULEX_ERROR_EEPREG_DISTORT		= 0x40;

// HerkuleX Broadcast ID
static byte HERKULEX_BROADCAST_ID = 0xFE;

class CHerkuleX {
public:
  void begin(long baudrate, uint8_t rx, uint8_t tx);
  void beginSerial(long baudrate);
#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega128__) || defined (__AVR_ATmega2560__)
  void beginSerial1(long baudrate);
  void beginSerial2(long baudrate);
  void beginSerial3(long baudrate);
#endif

  void torqueOn(uint8_t id);
  void torqueOff(uint8_t id);

  void turn(uint8_t id, int16_t pwmValue, uint8_t playtime = 0x30, uint8_t led = 0x00);
  void movePos(uint8_t id, uint16_t pos, uint8_t playtime = 0x30, uint8_t led = 0x00);
  void moveAngle(uint8_t id, float angle, uint8_t playtime = 0x30, uint8_t led = 0x00);
  void clear(uint8_t id);

  int16_t getTurnSpeed(uint8_t id);
  uint16_t getPos(uint8_t id);
  float getAngle(uint8_t id);
  //String readPos(uint8_t id);
  byte getStatus(uint8_t id);

private:
  void send(byte* buf, uint8_t size);
  void receive(uint8_t size);

  uint8_t comm_type;
  byte send_buf[BUFFER_SIZE];
  byte recv_buf[BUFFER_SIZE];
};

extern CHerkuleX HerkuleX;

#endif    // _HERKUELX_H
