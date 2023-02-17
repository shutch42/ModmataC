#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <modbus.h>
#include <errno.h>
#include <time.h>

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
#define WIRE_BEGIN 11
#define WIRE_BEGIN_TRANSMISSION 12
#define WIRE_END_TRANSMISSION 13
#define WIRE_WRITE 14
#define WIRE_REQUEST 15
#define WIRE_READ 16
#define WIRE_AVAILABLE 17

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

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
@param (int) miliseconds
@return void
*/
void delay(int ms);

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
void pinMode(int pinNum, int mode);

/**
Writes a digital value (1 or 0) to a pin
@param (int) pin number
@param (int) value to be written
@return void
*/
void digitalWrite(int pinNum, int input);

/**
Reads the value of a digital pin
@param (int) pin number
@return (int) value read from pin
*/
int digitalRead(int pinNum);

/**
Writes an analog value to a pin
@param (int) pin number
@param (int) value to be written
@return void
*/
void analogWrite(int pinNum, int input);

/**
Reads the value of an analog pin
@param (int) pin number
@return (int) value read from pin
*/
int analogRead(int pinNum);

/**
Attaches a servo to a pin
@param (int) pin number
@return void
*/
void servoAttach(int pinNum);

/**
Detaches a servo from a pin
@param (int) pin number
@return void
*/
void servoDetach(int pinNum);

/**
Writes an angle value to a servo
@param (int) pin number
@param (int) angle value to write
@return void
*/
void servoWrite(int pinNum, int input);

/**
Reads the value last written to a servo
@param (int) pin number
@return (int) angle value last written
*/
int servoRead(int pinNum);

void wireWrite(uint8_t addr, uint8_t reg, uint8_t num_bytes, uint8_t* data);
uint8_t* wireRead(uint8_t addr, uint8_t reg, int num_bytes);
