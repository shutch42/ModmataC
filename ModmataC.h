#include <stdio.h>
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

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

static modbus_t* arduino;

//  handles SIGINT signal and disconnects modbus before exiting
static void exit(int signal);

//  disconnect modbus connection if the program does not have an infinite loop
void closeConnection();

//  creates a system delay before moving to the next line 
void delay(int ms);

//  checks if pinNum is valid (1-30)
int isValidPin(int pinNum);

//  begins serial connection with arduino
int connectArduino(char *port, int baudRate, int id);

//  sets mode of a given pin
void pinMode(int pinNum, int mode);

//  write/read values to/from a digital pin
void digitalWrite(int pinNum, int input);
int digitalRead(int pinNum);

//  write/read values to/from a pwm pin
void analogWrite(int pinNum, int input);
int analogRead(int pinNum);

//  attach/detach servo to/from a pin
void servoAttach(int pinNum);
void servoDetach(int pinNum);

//  write/read values to/from a servo assigned pin
void servoWrite(int pinNum, int input);
int servoRead(int pinNum);
