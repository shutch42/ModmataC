#include <stdio.h>
#include <modbus.h>
#include <errno.h>

#define IDLE 0
#define PINMODE 1
#define DIGITALWRITE 2
#define DIGITALREAD 3
#define ANALOGREFERENCE 4
#define ANALOGWRITE 5
#define ANALOGREAD 6

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

//  checks if pinNum is valid (1-30)
int isValidPin(int pinNum);

//  begins serial connection with arduino
modbus_t* connectArduino(char *port, int baudRate, int id);

//  sets mode of a given pin
void pinMode(modbus_t *arduino, int pinNum, int mode);

//  write/read values to/from a digital pin
void digitalWrite(modbus_t *arduino, int pinNum, int input);
int digitalRead(modbus_t *arduino, int pinNum);

//  write/read values to/from a pwm pin
void analogWrite(modbus_t *arduino, int pinNum, int input);
int analogRead(modbus_t *arduino, int pinNum);
