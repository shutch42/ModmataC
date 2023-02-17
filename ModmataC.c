/*

A communications library in the C language for
Modmata, a practical combination of the Firmata 
and Modbus communication protocols!

For use on the Arduino Leonardo board:
    https://docs.arduino.cc/hardware/leonardo

Modmata (comm. protocol) repo:
    https://github.com/shutch42/modmata

ModmataC (C library) repo:
    https://github.com/shutch42/ModmataC

Authors:
    Sam Hutcherson
    Iris Astrid
    Chase Wallendorff

Last Updated:
    01/23/2023

*/

#include "ModmataC.h"

static void safe_exit(int sig)
{
    modbus_close(arduino);
    modbus_free(arduino);
    signal(sig, SIG_DFL);
    raise(sig);
}

void closeConnection() {
    modbus_close(arduino);
    modbus_free(arduino);
}

void delay(int ms)
{
   clock_t start_time = clock();
   while(clock() < start_time + ms*1000);
}

//  checks if a pin is valid, based on the
//  
//  valid pins are 1-30
int isValidPin(int pinNum)
{
    //  check if pin number is valid
    if (pinNum < 1 || pinNum > 30)
    {
        printf("invalid pin number\n");
        return 0;
    }

    return 1;
}

//  start serial connection using specified port and baud rate
//  no parity bits, 8 data bits, 1 stop bit
int connectArduino(char *port, int baudRate, int id)
{
    arduino = modbus_new_rtu(port, baudRate, 'N', 8, 1);
    if(!arduino) {
    	fprintf(stderr, "unable to create the libmodbus context\n");
	return 0;
    }
    if(modbus_set_slave(arduino, id) == -1) {
    	fprintf(stderr, "Invalid slave ID\n");
        modbus_free(arduino);
	return 0;
    }
    if(modbus_connect(arduino) == -1) {
    	fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(arduino);
	return 0;
    }
    signal(SIGINT, safe_exit);
    signal(SIGTERM, safe_exit);
    return 1;
}

// sets pin mode
void pinMode(int pinNum, int mode)
{
    //  check if pin number is valid
    if (isValidPin(pinNum))
    {
        //  set pin mode
        uint16_t command[3] = {PINMODE, pinNum, mode};
        modbus_write_registers(arduino, 0, 3, command);

        //  TODO: make sure mode is valid

        return;
    }
}

//  writes a HIGH or LOW value to a digital pin
void digitalWrite(int pinNum, int input)
{
    //  ensures valid pin number and that input is either 0 (LOW) or 1 (HIGH)
    if (isValidPin(pinNum) && (input == 0 || input == 1))
    {
        uint16_t command[3] = {DIGITALWRITE, pinNum, input};
        modbus_write_registers(arduino, 0, 3, command);
    }

    return;
}

//  reads the value of a digital pin
int digitalRead(int pinNum)
{

    if (isValidPin(pinNum))
    {
        //  tell the arduino we're reading a value
        uint16_t command[2] = {DIGITALREAD, pinNum};
        modbus_write_registers(arduino, 0, 2, command);

        //  read the value that should now be in address 2
        uint16_t dest;
        modbus_read_registers(arduino, 2, 1, &dest);

        int value = dest;

        return value;
    }

    //  return -1 if invalid pin
    return -1;
}

//  writes an analog value (PWM wave, 0-255) to a pin
void analogWrite(int pinNum, int input)
{
    //  TODO: make sure pin is also analog pin

    //  ensures valid pin number and input is between 0 and 255
    if (isValidPin(pinNum) && !(input < 0 || input > 255))
    {
        uint16_t command[3] = {ANALOGWRITE, pinNum, input};
        modbus_write_registers(arduino, 0, 3, command);
    }

    return;
}

// reads the value of an analog (PWM) pin
int analogRead(int pinNum)
{
    if (isValidPin(pinNum))
    {
        //  tell the arduino we're reading a value
        uint16_t command[2] = {ANALOGREAD, pinNum};
        modbus_write_registers(arduino, 0, 2, command);

        //  read the value that should now be in address 2
        uint16_t dest;
        modbus_read_registers(arduino, 2, 1, &dest);

        int value = dest;

        return value;
    }

    //  return -1 if invalid pin
    return -1;
}

//  attach servo to a pin
//  TODO: add validation for pwm pin
void servoAttach(int pinNum)
{
    uint16_t command[2] = {SERVOATTACH, pinNum};
    modbus_write_registers(arduino, 0, 2, command);
    return;
}

//  detach servo from a pin
void servoDetach(int pinNum)
{
    uint16_t command[2] = {SERVODETACH, pinNum};
    modbus_write_registers(arduino, 0, 2, command);
    return;
}

//  write/read values to/from a servo assigned pin
//  TODO: add validation for pwm pin
void servoWrite(int pinNum, int input)
{
    //  ensures valid pin number and input is between 0 and 180
    if (isValidPin(pinNum) && !(input < 0 || input > 180))
    {
        uint16_t command[3] = {SERVOWRITE, pinNum, input};
        modbus_write_registers(arduino, 0, 3, command);
    }
    return;
}

//  read value from a servo assigned pin
//  returns 0 if fails
int servoRead(int pinNum)
{
    if (isValidPin(pinNum))
    {
        //  tell the arduino we're reading a value
        uint16_t command[2] = {SERVOREAD, pinNum};
        modbus_write_registers(arduino, 0, 2, command);

        //  read the value that should now be in address 2
        uint16_t dest;
        modbus_read_registers(arduino, 2, 1, &dest);

        int value = dest;

        return value;
    }

    //  return -1 if invalid pin
    return -1;
}

void wireWrite(uint8_t addr, uint8_t reg, uint8_t num_bytes, uint8_t *data) {
	uint16_t wait_signal = 1;
	do {
		modbus_read_registers(arduino, 0, 1, &wait_signal);	
	} while(wait_signal);

	uint16_t *command = malloc(sizeof(uint16_t) * (4 + num_bytes));
	command[0] = 100;
	command[1] = 2 + num_bytes;
	command[2] = addr;
	command[3] = reg;
	for (int i = 0; i < num_bytes; i++) {
		command[4+i] = data[i];
	}
	modbus_write_registers(arduino, 0, 4 + num_bytes, command);
}

uint8_t* wireRead(uint8_t addr, uint8_t reg, int num_bytes) {
	uint16_t wait_signal = 1;
	do {
		modbus_read_registers(arduino, 0, 1, &wait_signal);	
	} while(wait_signal);
	
	uint16_t command[5] = {101, 3, addr, reg, num_bytes};
	modbus_write_registers(arduino, 0, 5, command);

	uint16_t *registers = malloc(sizeof(uint16_t) * num_bytes);
	modbus_read_registers(arduino, 2, num_bytes, registers);
	
	uint8_t *result = malloc(sizeof(uint8_t) * num_bytes);
	for(int i = 0; i < num_bytes; i++) {
		result[i] = (uint8_t)registers[i];
	}
	
	return result;
}

