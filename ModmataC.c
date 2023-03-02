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

Modbus, might be useful for reference:
    https://libmodbus.org/reference/

Authors:
    Sam Hutcherson
    Iris Astrid
    Chase Wallendorff

Last Updated:
    01/23/2023

*/

/*
a note on our modified modbus command[] format:
        command[0]  = command value (1 for pinmode, 2 for digital write, etc)
        command[1]  = num of arguments included in the command
  command[3 ... n]  = command arguments
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

void delay(int millis)
{
   clock_t start_time = clock();
   while(clock() < start_time + millis*1000);
}

void delayMicroseconds(int micros) {
   clock_t start_time = clock();
   while(clock() < start_time + micros);
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
void pinMode(uint8_t pinNum, uint8_t mode)
{
    //  check if pin number is valid
    if (isValidPin(pinNum))
    {
        uint8_t numArgs = 2;
        //  set pin mode
        uint16_t command[2] = {(PINMODE << 8) | numArgs, (pinNum << 8) | mode};
        modbus_write_registers(arduino, 0, 2, command);

        //  TODO: make sure mode is valid

        return;
    }
}

//  writes a HIGH or LOW value to a digital pin
void digitalWrite(uint8_t pinNum, uint8_t input)
{
    //  ensures valid pin number and that input is either 0 (LOW) or 1 (HIGH)
    if (isValidPin(pinNum) && (input == 0 || input == 1))
    {
        uint8_t numArgs = 2;
        uint16_t command[2] = {(DIGITALWRITE << 8) | numArgs, (pinNum << 8) | input};
        modbus_write_registers(arduino, 0, 2, command);
    }

    return;
}

//  reads the value of a digital pin
int digitalRead(uint8_t pinNum)
{
    if (isValidPin(pinNum))
    {
        uint8_t numArgs = 1;

        //  tell the arduino we're reading a value
        uint16_t command[2] = {(DIGITALREAD << 8) | numArgs, pinNum << 8};
        modbus_write_registers(arduino, 0, 2, command);

        //  read the value that should now be in register 2
        uint16_t dest;
        modbus_read_registers(arduino, 1, 1, &dest);

        int value = dest >> 8;

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
        int numArgs = 2;
        uint16_t command[4] = {ANALOGWRITE, numArgs, pinNum, input};
        modbus_write_registers(arduino, 0, 4, command);
    }

    return;
}

// reads the value of an analog (PWM) pin
int analogRead(int pinNum)
{
    if (isValidPin(pinNum))
    {
        int numArgs = 1;

        //  tell the arduino we're reading a value
        uint16_t command[3] = {ANALOGREAD, numArgs, pinNum};
        modbus_write_registers(arduino, 0, 3, command);

        //  read the value that should now be in address 3
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
    int numArgs = 1;
    uint16_t command[3] = {SERVOATTACH, numArgs, pinNum};
    modbus_write_registers(arduino, 0, 3, command);
    return;
}

//  detach servo from a pin
void servoDetach(int pinNum)
{
    int numArgs = 1;
    uint16_t command[3] = {SERVODETACH, numArgs, pinNum};
    modbus_write_registers(arduino, 0, 3, command);
    return;
}

//  write/read values to/from a servo assigned pin
//  TODO: add validation for pwm pin
void servoWrite(int pinNum, int input)
{
    //  ensures valid pin number and input is between 0 and 180
    if (isValidPin(pinNum) && !(input < 0 || input > 180))
    {
        int numArgs = 2;
        uint16_t command[4] = {SERVOWRITE, numArgs, pinNum, input};
        modbus_write_registers(arduino, 0, 4, command);
    }
    return;
}

//  read value from a servo assigned pin
//  returns 0 if fails
int servoRead(int pinNum)
{
    if (isValidPin(pinNum))
    {
        int numArgs = 1;

        //  tell the arduino we're reading a value
        uint16_t command[3] = {SERVOREAD, numArgs, pinNum};
        modbus_write_registers(arduino, 0, 3, command);

        //  read the value that should now be in address 2
        uint16_t dest;
        modbus_read_registers(arduino, 3, 1, &dest);

        int value = dest;

        return value;
    }

    //  return -1 if invalid pin
    return -1;
}

void wireBegin() {
	uint16_t command[2] = {WIREBEGIN, 0};
	modbus_write_registers(arduino, 0, 2, command);
}

void wireEnd() {
	uint16_t command[2] = {WIREEND, 0};
	modbus_write_registers(arduino, 0, 2, command);
}

void wireSetClock(int clock_speed) {
	uint16_t command[3] = {WIRECLOCK, 1, clock_speed};
	modbus_write_registers(arduino, 0, 3, command);
}

void wireWrite(uint8_t addr, uint8_t reg, uint8_t num_bytes, uint8_t *data) {
	uint16_t *command = malloc(sizeof(uint16_t) * (4 + num_bytes));
	command[0] = WIREWRITE;
	command[1] = 2 + num_bytes;
	command[2] = addr;
	command[3] = reg;
	for (int i = 0; i < num_bytes; i++) {
		command[4+i] = data[i];
	}
	modbus_write_registers(arduino, 0, 4 + num_bytes, command);
}

uint8_t* wireRead(uint8_t addr, uint8_t reg, int num_bytes) {
	uint16_t command[5] = {WIREREAD, 3, addr, reg, num_bytes};
	modbus_write_registers(arduino, 0, 5, command);

	uint16_t *registers = malloc(sizeof(uint16_t) * num_bytes);
	modbus_read_registers(arduino, 2, num_bytes, registers);
	
	uint8_t *result = malloc(sizeof(uint8_t) * num_bytes);
	for(int i = 0; i < num_bytes; i++) {
		result[i] = (uint8_t)registers[i];
	}

	free(registers);

	return result;
}

void spiBegin() {
	uint16_t command[1] = {SPIBEGIN << 8};
	modbus_write_registers(arduino, 0, 1, command);
}

uint8_t* spiTransferBuf(int CS_pin, uint8_t *buf, uint8_t length) {
	uint16_t *command = malloc(sizeof(uint16_t) * (2 + length/2));
	command[0] = (SPITRANSFER << 8) | (length + 1);
	command[1] = CS_pin << 8;
	for (int i = 0; i < length; i++) {
		if (i % 2 == 0) {
			command[(i+1)/2 + 1] |= buf[i];
		}
		else {
			command[(i+1)/2 + 1] = buf[i] << 8;
		}
	}
	modbus_write_registers(arduino, 0, 2 + length/2, command);

	uint16_t *registers = malloc(sizeof(uint16_t) * (length + 1) / 2);
	modbus_read_registers(arduino, 1, (length + 1) / 2, registers);
	
	uint8_t *result = malloc(sizeof(uint8_t) * length);
	for(int i = 0; i < length; i++) {
		if (i % 2 == 0) {
			result[i] = registers[i/2] >> 8;
		}
		else {
			result[i] = registers[i/2] & 0x00ff;
		}
	}

	free(registers);

	return result;	
}

