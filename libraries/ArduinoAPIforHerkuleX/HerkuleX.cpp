/*
  HerkuleX.cpp - Dongbu Robot HerkuleX Servo library for Arduino - Version 0.1
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

  See. HerkuleX.h
*/

#include "HerkuleX.h"
#include "SoftwareSerial.h"

// HerekuleX Command - See. Manual p40.
#define DR_SERVO_CMD_ROM_WRITE   0x01
#define DR_SERVO_CMD_ROM_READ    0x02
#define DR_SERVO_CMD_RAM_WRITE	 0x03
#define DR_SERVO_CMD_RAM_READ	 0x04
#define DR_SERVO_CMD_I_JOG		 0x05
#define DR_SERVO_CMD_S_JOG		 0x06
#define DR_SERVO_CMD_STAT	 	 0x07
#define DR_SERVO_CMD_ROLLBACK	 0x08
#define DR_SERVO_CMD_REBOOT	 	 0x09

extern SoftwareSerial SoftSerial(0, 1);
CHerkuleX HerkuleX;

enum {
	HARDWARE_SERIAL 	= 0,
	HARDWARE_SERIAL1 	= 1,
	HARDWARE_SERIAL2 	= 2,
	HARDWARE_SERIAL3 	= 3,
	SOFTWARE_SERIAL 	= 4,
};

/**
 * Begin HerkuleX Servo control using SoftwareSerial library
 * All Arduino boards are available with this function
 * Arduino Uno is only avaialbe with SoftwareSerial
 *
 * void begin(long baudrate, uint8_t rx, uint8_t tx)
 *
 * baudrate - 57600(recommended)
 * rx - Arduino board RX (This pin is connected with HerkuleX servo TX)
 * tx - Arduino board TX (This pin is connected with HerkuleX servo RX)
 */
void CHerkuleX::begin(long baudrate, uint8_t rx, uint8_t tx)
{
	SoftSerial.setRX(rx);
	SoftSerial.setTX(tx);
	SoftSerial.begin(baudrate);
	comm_type = SOFTWARE_SERIAL;
}

/**
 * Begin HerkuleX Servo control using Serial
 * All Arduino boards are available with this function
 * BUT, usually Serial pins are being used for communicationg with PC.
 * You might rarely use this function.
 *
 * void beginSerial(long baudrate)
 *
 * baudrate - baudrate
 */
void CHerkuleX::beginSerial(long baudrate)
{
	Serial.begin(baudrate);
	comm_type = HARDWARE_SERIAL;
}

/**
 * Begin HerkuleX Servo control using Serial1, Serial2, and Serial3
 * This function is available with Arduino Mega and Due (chips based on ATmega1280 and 2560)
 *
 * void beginSerial1(long baudrate)
 * void beginSerial2(long baudrate)
 * void beginSerial3(long baudrate)
 *
 * baudrate - 115200(recommneded)
 */
#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega128__) || defined (__AVR_ATmega2560__)
void CHerkuleX::beginSerial1(long baudrate)
{
	Serial1.begin(baudrate);
	comm_type = HARDWARE_SERIAL1;
}

void CHerkuleX::beginSerial2(long baudrate)
{
	Serial2.begin(baudrate);
	comm_type = HARDWARE_SERIAL2;
}

void CHerkuleX::beginSerial3(long baudrate)
{
	Serial3.begin(baudrate);
	comm_type = HARDWARE_SERIAL3;
}
#endif

/**
 * Sending the size of bytes from the buffer to Serial line
 *
 * void send(byte* buf, uint8_t size)
 *
 * buf - byte array
 * size - the size of sending bytes
 */
void CHerkuleX::send(byte* buf, uint8_t size)
{
	switch (comm_type)
	{
	case SOFTWARE_SERIAL:
		SoftSerial.write(buf, size);
		break;
	case HARDWARE_SERIAL:
		Serial.write(buf, size);
		break;
#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega128__) || defined (__AVR_ATmega2560__)
	case HARDWARE_SERIAL1:
		Serial1.write(buf, size);
		break;
	case HARDWARE_SERIAL2:
		Serial2.write(buf, size);
		break;
	case HARDWARE_SERIAL3:
		Serial3.write(buf, size);
		break;
#endif
	}
}

/**
 * Receiving the size of bytes from Serial line
 *
 * void receive(uint8_t size)
 *
 * size - the size of receiving bytes
 */
void CHerkuleX::receive(uint8_t size)
{
	uint8_t i = 0;
	bool isFirstHeader = false;
	bool isSecondHeader = false;

	switch (comm_type)
	{
	case SOFTWARE_SERIAL:
		while (SoftSerial.available()) {
			byte inchar = (byte)SoftSerial.read();
			if (i < size) recv_buf[i] = inchar;
			//delay(1);
			i++;
		}
		SoftSerial.flush();
		break;
	case HARDWARE_SERIAL:
		while (Serial.available()) {
			byte inchar = (byte)Serial.read();
			if (i < size) recv_buf[i] = inchar;
			//delay(1);
			i++;
		}
		Serial.flush();
		break;
#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega128__) || defined (__AVR_ATmega2560__)
	case HARDWARE_SERIAL1:
		while (Serial1.available()) {
			byte inchar = (byte)Serial1.read();
			if (i < size) recv_buf[i] = inchar;
			//delay(1);
			i++;
		}
		Serial1.flush();
		break;
	case HARDWARE_SERIAL2:
		while (Serial2.available()) {
			byte inchar = (byte)Serial2.read();
			if (i < size) recv_buf[i] = inchar;
			//delay(1);
			i++;
		}
		Serial2.flush();
		break;
	case HARDWARE_SERIAL3:
		while (Serial3.available()) {
			byte inchar = (byte)Serial3.read();
			if (i < size) recv_buf[i] = inchar;
			//delay(1);
			i++;
		}
		Serial3.flush();
		break;
#endif
	}
}

/**
 * Torque on HerkuleX Servo by ID
 *
 * void torqueOn(uint8_t id)
 *
 * id - HerkuleX Servo ID (Broadcast ID 0xFE)
 *
 * If you set 0xFE to this parameter, All servos torque on
 *
 */
void CHerkuleX::torqueOn(uint8_t id)
{
	send_buf[0] = 0xFF;						// Packet Header
	send_buf[1] = 0xFF;
	send_buf[2] = 0x0A;	 					// Packet Size
	send_buf[3] = id;  						// Servo ID
	send_buf[4] = DR_SERVO_CMD_RAM_WRITE;  	// Command Ram Write
	send_buf[7] = 0x34; 					// Address 52
	send_buf[8] = 0x01; 					// Length
	send_buf[9] = 0x60; 					// Torque ON

	send_buf[5] = (send_buf[2]^send_buf[3]^send_buf[4]^send_buf[7]^send_buf[8]^send_buf[9]) & 0xFE;
	send_buf[6] = (~send_buf[5])&0xFE;

	send(send_buf, 10);
}

/**
 * Torque off HerkuleX Servo by ID
 *
 * void torqueOff(uint8_t id)
 *
 * id - HerkuleX Servo ID (Broadcast ID 0xFE)
 *
 * If you set 0xFE to this parameter, All servos torque off
 *
 */
void CHerkuleX::torqueOff(uint8_t id)
{
	send_buf[0] = 0xFF;						// Packet Header
	send_buf[1] = 0xFF;
	send_buf[2] = 0x0A;	 					// Packet Size
	send_buf[3] = id;  						// Servo ID
	send_buf[4] = DR_SERVO_CMD_RAM_WRITE;  	// Command Ram Write
	send_buf[7] = 0x34; 					// Address 52
	send_buf[8] = 0x01; 					// Length
	send_buf[9] = 0x00; 					// Torque OFF

	send_buf[5] = (send_buf[2]^send_buf[3]^send_buf[4]^send_buf[7]^send_buf[8]^send_buf[9]) & 0xFE;
	send_buf[6] = (~send_buf[5])&0xFE;

	send(send_buf, 10);
}

/**
 * Infinite turn HerkuleX Servo by pwmValue (-1023 ~ 1023)
 *
 * void turn(uint8_t id, int16_t pwmValue, uint8_t playtime, uint8_t led)
 *
 * id - HerkuleX Servo ID (Broadcast ID 0xFE)
 * pwmValue - Turn speed (-1023 ~ 1023)
 *   		  Negative value - CW(Clock Wise) Positive value - CCW(Counter Clock Wise)
 * playtime - 0x01 - 11.2ms (unmeaningful in turn())
 * led - Red - 0x01, Green - 0x04, Blue - 0x08
 *
 */
void CHerkuleX::turn(uint8_t id, int16_t pwmValue, uint8_t playtime, uint8_t led)
{
	if (pwmValue > 1023 || pwmValue < -1023) return;
	if (playtime > 255) return;

	send_buf[0] = 0xFF;						// Packet Header
	send_buf[1] = 0xFF;
	send_buf[2] = 0x0C;						// Packet Size
	send_buf[3] = id;						// Servo ID
	send_buf[4] = DR_SERVO_CMD_S_JOG;		// Command S_JOG

	send_buf[7] = playtime;					// Playtime

	int16_t temp;
	if (pwmValue < 0) {
		temp = pwmValue * -1;
		temp |= 0x4000;
	} else {
		temp = pwmValue;
	}
	send_buf[8] = temp & 0x00FF;						// JOG(LSB)
	send_buf[9] = (temp & 0xFF00) >> 8;						// JOG(MSB)
	send_buf[10] = led | 0x02;				// LED Red ON
	send_buf[11] = id;   		// Servo ID

	// Check Sum1 and Check Sum2
	send_buf[5] = (send_buf[2]^send_buf[3]^send_buf[4]^send_buf[7]^send_buf[8]^send_buf[9]^
								send_buf[10]^send_buf[11]) & 0xFE;
	send_buf[6] = (~send_buf[5])&0xFE;

	send(send_buf, 12);
}

/**
 * Get current turn speed
 *
 * int16_t getTurnSpeed(uint8_t id)
 *
 * id - HerkuleX Servo ID
 *
 * RETURN
 *   - Current PWM Value (-1023~1023);
 */
int16_t CHerkuleX::getTurnSpeed(uint8_t id) {
	int16_t pwmValue = 0;

	send_buf[0] = 0xFF;
	send_buf[1] = 0xFF;
	send_buf[2] = 0x09;
	send_buf[3] = id;
	send_buf[4] = DR_SERVO_CMD_RAM_READ;

	send_buf[7] = 0x40;
	send_buf[8] = 0x02;

	// Check Sum1 and Check Sum2
	send_buf[5] = (send_buf[2]^send_buf[3]^send_buf[4]^send_buf[7]^send_buf[8]) & 0xFE;
	send_buf[6] = (~send_buf[5])&0xFE;

	send(send_buf, 9);
	delay(1);
	receive(13);

	// Check Sum1 and Check Sum2
	byte chksum1 = (recv_buf[2]^recv_buf[3]^recv_buf[4]^recv_buf[7]^recv_buf[8]^
					recv_buf[9]^recv_buf[10]^recv_buf[11]^recv_buf[12]) & 0xFE;
	if (chksum1 != recv_buf[5]) return -1;
	byte chksum2 = (~recv_buf[5]&0xFE);
	if (chksum2 != recv_buf[6]) return -1;

	pwmValue = ((recv_buf[10]&0xFF)<<8) | recv_buf[9];

	return pwmValue;
}

/**
 * Move HerkuleX servo to target position
 *
 * void movePos(uint8_t id, uint16_t pos, uint8_t playtime, uint8_t led)
 *
 * id : HerkuleX Servo ID
 * pos : Target position (0~1023) See. Manual p33.
 * playtime : Time to target position. (playtime * 11.2)ms. 1 - 11.2ms 2 - 22.4ms ...
 * led - Red - 0x01, Green - 0x04, Blue - 0x08
 */
void CHerkuleX::movePos(uint8_t id, uint16_t pos, uint8_t playtime, uint8_t led)
{
	if (pos > 1023 || pos < 0) return;
	if (playtime > 255) return;

	send_buf[0] = 0xFF;
	send_buf[1] = 0xFF;
	send_buf[2] = 0x0C;						// Packet Size
	send_buf[3] = id;			// Servo id
	send_buf[4] = DR_SERVO_CMD_S_JOG;		// Command S_JOG

	send_buf[7] = playtime;						// Play time
	send_buf[8] = pos & 0x00FF;	// JOG(LSB)
	send_buf[9] = (pos & 0xFF00) >> 8;					// JOG(MSB)
	send_buf[10] = led & 0xFD;				// LED Red ON
	send_buf[11] = id;   		// Servo ID

	// Check Sum1 and Check Sum2
	send_buf[5] = (send_buf[2]^send_buf[3]^send_buf[4]^send_buf[7]^send_buf[8]^send_buf[9]^
								send_buf[10]^send_buf[11]) & 0xFE;
	send_buf[6] = (~send_buf[5])&0xFE;

	send(send_buf, 12);
}

/**
 * Get current position of HerkuelX Servos
 *
 * uint16_t getPos(uint8_t id)
 *
 * id - HerkuleX Servo ID
 *
 * RETURN
 *   - Current position (0 ~ 1023) See. Manual p33.
 */
uint16_t CHerkuleX::getPos(uint8_t id) {
	uint16_t pos  = 0;

	send_buf[0] = 0xFF;
	send_buf[1] = 0xFF;
	send_buf[2] = 0x09;
	send_buf[3] = id;
	send_buf[4] = DR_SERVO_CMD_RAM_READ;

	send_buf[7] = 0x3A;
	send_buf[8] = 0x02;

	// Check Sum1 and Check Sum2
	send_buf[5] = (send_buf[2]^send_buf[3]^send_buf[4]^send_buf[7]^send_buf[8]) & 0xFE;
	send_buf[6] = (~send_buf[5])&0xFE;

	send(send_buf, 9);
	delay(1);
	receive(13);

	// Check Sum1 and Check Sum2
	byte chksum1 = (recv_buf[2]^recv_buf[3]^recv_buf[4]^recv_buf[7]^recv_buf[8]^
					recv_buf[9]^recv_buf[10]^recv_buf[11]^recv_buf[12]) & 0xFE;
	if (chksum1 != recv_buf[5]) return -1;
	byte chksum2 = (~recv_buf[5]&0xFE);
	if (chksum2 != recv_buf[6]) return -1;

	pos = ((recv_buf[10]&0x03)<<8) | recv_buf[9];

	return pos;
}

/**
 * Move HerkuleX Servo by angle(degree value - NOT Radian) (-159.8~159.8)
 * (Degree = Position Value * 0.325)
 *
 * void moveAngle(uint8_t id, float angle, uint8_t playtime, uint8_t led)
 *
 * id - HerkuleX Servo ID
 * angle - (-159.8 ~ 159.8) See. Manual p31~32
 * playtime : Time to target position. (playtime * 11.2)ms. 1 - 11.2ms 2 - 22.4ms ...
 * led - Red - 0x01, Green - 0x04, Blue - 0x08
 *
 */
void CHerkuleX::moveAngle(uint8_t id, float angle, uint8_t playtime, uint8_t led) {
	if (angle > 166.7 || angle < -166.7) return;	// Full range
	//if (angle > 159.8 || angle < -159.8) return; 		// Recommended range
	if (playtime > 255) return;

	uint16_t pos = (uint16_t)(angle/0.325f) + 512;
	movePos(id, pos, playtime, led);
}

/**
 * Get current angle(degree value - NOT Radian)
 * (Degree = Position Value * 0.325)
 *
 * void getAngle(uint8_t id)
 *
 * id - HerkuleX Servo ID
 *
 * RETURN
 * 	- Current angle (degree)
 */
float CHerkuleX::getAngle(uint8_t id) {
	int16_t pos = (int16_t)getPos(id);
	return (pos-512) * 0.325f;
}

/**
 * Get current HerkuleX Servo status
 *
 * byte getStatus(uint8_t id)
 *
 * id - HerkuleX Servo ID
 *
 * RETURN
 *  Current servo status - See. Manual p39.
 *
 *  HERKULEX_STATUS_OK					= 0x00;
 *	HERKULEX_ERROR_INPUT_VOLTAGE 		= 0x01;
 *  HERKULEX_ERROR_POS_LIMIT			= 0x02;
 *  HERKULEX_ERROR_TEMPERATURE_LIMIT	= 0x04;
 *  HERKULEX_ERROR_INVALID_PKT			= 0x08;
 *  HERKULEX_ERROR_OVERLOAD				= 0x10;
 *  HERKULEX_ERROR_DRIVER_FAULT  		= 0x20;
 *  HERKULEX_ERROR_EEPREG_DISTORT		= 0x40;
 */
byte CHerkuleX::getStatus(uint8_t id) {
	byte status;

	send_buf[0] = 0xFF;
	send_buf[1] = 0xFF;
	send_buf[2] = 0x07;
	send_buf[3] = id;
	send_buf[4] = DR_SERVO_CMD_STAT;

	// Check Sum1 and Check Sum2
	send_buf[5] = (send_buf[2]^send_buf[3]^send_buf[4]) & 0xFE;
	send_buf[6] = (~send_buf[5])&0xFE;

	int i = 0;

	send(send_buf, 7);
	delay(1);
	receive(9);

	// Check Sum1 and Check Sum2
	byte chksum1 = (recv_buf[2]^recv_buf[3]^recv_buf[4]^recv_buf[7]^recv_buf[8]) & 0xFE;
	if (chksum1 != recv_buf[5]) return -1;
	byte chksum2 = (~recv_buf[5]&0xFE);
	if (chksum2 != recv_buf[6]) return -1;

	status = recv_buf[7];

	return status;
}

/**
 * Clear HerkuleX Servo
 * Get rid of HerkuleX Servo error status
 *
 * void clear(uint8_t id)
 *
 * id - HerkuleX Servo ID
 */
void CHerkuleX::clear(uint8_t id) {
	send_buf[0] = 0xFF;
	send_buf[1] = 0xFF;
	send_buf[2] = 0x0B;
	send_buf[3] = id;
	send_buf[4] = DR_SERVO_CMD_RAM_WRITE;

	send_buf[7] = 0x30;
	send_buf[8] = 0x02;
	send_buf[9] = 0x00;
	send_buf[10] = 0x00;

	// Check Sum1 and Check Sum2
	send_buf[5] = (send_buf[2]^send_buf[3]^send_buf[4]^send_buf[7]^send_buf[8]^send_buf[9]^
								send_buf[10]) & 0xFE;
	send_buf[6] = (~send_buf[5])&0xFE;

	send(send_buf, 11);
}

//String CHerkuleX::readPos(uint8_t id) {
//	int pos  = 0;
//
//	send_buf[0] = 0xFF;
//	send_buf[1] = 0xFF;
//	send_buf[2] = 0x09;
//	send_buf[3] = id;
//	send_buf[4] = DR_SERVO_CMD_RAM_READ;
//
//	send_buf[7] = 0x3A;
//	send_buf[8] = 0x02;
//
//	// Check Sum1 and Check Sum2
//	send_buf[5] = (send_buf[2]^send_buf[3]^send_buf[4]^send_buf[7]^send_buf[8]) & 0xFE;
//	send_buf[6] = (~send_buf[5])&0xFE;
//
//
//	//SoftSerial.write(send_buf, 9);
//	Serial3.write(send_buf, 9);
//	//delay(1);
//	//while (SoftSerial.available()) {
//	int i = 0;
//	String inString = "";
//	bool isFirstHeader = false;
//	bool isSecondHeader = false;
//	while (Serial3.available()) {
//		//byte inchar = (byte)SoftSerial.read();
//		byte inchar = (byte)Serial3.read();
//		inString += inchar;
//		inString += " ";
//
////		if (inchar == 0xFF && !isFirstHeader && !isSecondHeader) isFirstHeader = true;
////		if (isFirstHeader) {
////			if (inchar==0xFF) {
////				inString += inchar;
////				inString += " ";
////				isSecondHeader = true;
////				isFirstHeader = false;
////			}
////		}
////		if (isSecondHeader) {
////			inString += inchar;
////			inString += " ";
////			recv_buf[i] = inchar;
////			i++;
////		}
//	}
//	//SoftSerial.flush();
//	Serial3.flush();
//
//	// Check Sum1 and Check Sum2
//	byte chksum1 = (recv_buf[2]^recv_buf[3]^recv_buf[4]^recv_buf[7]^recv_buf[8]^
//					recv_buf[9]^recv_buf[10]^recv_buf[11]^recv_buf[12]) & 0xFE;
//	//if (chksum1 != recv_buf[5]) return -1;
//	byte chksum2 = (~recv_buf[5]&0xFE);
//	//if (chksum2 != recv_buf[6]) return -1;
//
//	pos = ((recv_buf[9]&0x03) <<8) | recv_buf[10];
//
//	return inString;
//}
