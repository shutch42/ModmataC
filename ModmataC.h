/*
ModmataC
Copyright © char* teamName <shutche@siue.edu>
Licensed under LGPL-2.1
*/

#ifndef MODMATA_H
#define MODMATA_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include "modbus/modbus.h"

#define IDLE 0
#define PINMODE 1
#define DIGITALWRITE 2
#define DIGITALREAD 3
#define ANALOGREFERENCE 4
#define ANALOGWRITE 5
#define ANALOGREAD 6
#define SERVOATTACH 7
#define SERVODETACH 8
#define SERVOWRITE 9
#define SERVOREAD 10
#define WIREBEGIN 11
#define WIREEND 12
#define WIRECLOCK 13
#define WIREWRITE 14
#define WIREREAD 15
#define SPIBEGIN 16
#define SPISETTINGS 17
#define SPITRANSFER 18
#define SPIEND 19

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

#define LSBFIRST 0
#define MSBFIRST 1

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

static modbus_t* arduino;

/**
handles SIGINT signal and disconnects modbus before exiting
@param (int) signal
@return void
*/
static void safe_exit(int signal);

/**
disconnect modbus connection if the program does not have an infinite loop
@param none
@return void
*/
void closeConnection();

/**
Causes the system to delay a number of miliseconds
@param (int) milliseconds
@return void
*/
void delay(int millis);

void delayMicroseconds(int micros);

/**
Determines if a given pin number is valid (valid pins are 1-30)
@param (int) pin number
@return (int) 1 if valid, 0 if invalid
*/
int isValidPin(int pinNum);

/**
Initiates connection between machine and Arduino
@param (char*) path to port
@param (int) baud rate (9600 default)
@param (int) slave id
@return (int) 1 if connection is successful, 0 if failure
*/
int connectArduino(char *port, int baudRate, int id);

/**
Sets the pin mode of a pin on the Arduino
@param (int) pin number
@param (int) mode to set pin to
@return void
*/
void pinMode(uint8_t pinNum, uint8_t mode);

/**
Writes a digital value (1 or 0) to a pin
@param (int) pin number
@param (int) value to be written
@return void
*/
void digitalWrite(uint8_t pinNum, uint8_t input);

/**
Reads the value of a digital pin
@param (int) pin number
@return (int) value read from pin
*/
int digitalRead(uint8_t pinNum);

/**
Writes an analog value to a pin
@param (int) pin number
@param (int) value to be written
@return void
*/
void analogWrite(uint8_t pinNum, uint16_t input);

/**
Reads the value of an analog pin
@param (int) pin number
@return (int) value read from pin
*/
int analogRead(uint16_t pinNum);

/**
Attaches a servo to a pin
@param (int) pin number
@return void
*/
void servoAttach(uint16_t pinNum);

/**
Detaches a servo from a pin
@param (int) pin number
@return void
*/
void servoDetach(uint16_t pinNum);

/**
Writes an angle value to a servo
@param (int) pin number
@param (int) angle value to write
@return void
*/
void servoWrite(uint16_t pinNum, uint16_t input);

/**
Reads the value last written to a servo
@param (int) pin number
@return (int) angle value last written
*/
int servoRead(uint16_t pinNum);

/**
Begins an I2C connection
@return (void)
*/
void wireBegin();

/**
Terminates an I2C connection
@return (void)
*/
void wireEnd();

/**
Sets I2C clock speed
@param (int) clock speed
@return (void)
*/
void wireSetClock(uint32_t);

/**
Writes data over an I2C connection
@param (uint8_t) address byte
@param (uint8_t) register byte
@param (uint8_t) number of bytes to be written following the address and register
@param (uint8_t*) array of bytes being written
@return (void)
*/
void wireWrite(uint8_t addr, uint8_t reg, uint8_t num_bytes, uint8_t* data);

/**
Reads data from an I2C connection
@param (uint8_t) address byte
@param (uint8_t) register byte
@param (uint8_t) number of bytes to read
@return (uint8_t*) array of bytes that were read
*/
uint8_t* wireRead(uint8_t addr, uint8_t reg, int num_bytes);

/**
Begins SPI communication with default settings (4 Mbps, MSBFIRST, SPI_MODE0)
@return (void)
*/
void spiBegin();

/**
Transfer an array of bytes over SPI
@param (int) Chip Select pin number
@param (uint8_t*) Array of bytes to transfer over MOSI
@param (uint8_t) Array length
@return (uint8_t*) Array of bytes sent back over MISO
*/
uint8_t* spiTransferBuf(int CS_pin, uint8_t *buf, uint8_t length);

/**
Set specific SPI settings such as communication speed, bit order, and mode
@param (uint32_t) Communication speed
@param (uint8_t) bit order (MSBFIRST or LSBFIRST)
@param (uint8_t) data mode (SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3)
@return (void)
*/
void spiSettings(uint32_t speed, uint8_t order, uint8_t mode);

/**
End SPI communication
@return (void)
*/
void spiEnd();

void transmitRegisters(uint8_t fn_code, uint8_t argc, uint8_t* argv);

#endif
