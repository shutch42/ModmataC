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

/**
Checks if a pin number is valid
The leonardo has pins 1-30
@param (int) pint number
@return (int) 1 if pin is valid, 0 if invalid
*/
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

/**
Start serial connection using specified port and baud rate
@param (char*) path to the port on which Leonardo is connected
@param (int) baud rate
@param (int) slave id for Leonardo
@return (int) 1 if successful, 0 if unsuccessful
*/
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

/**
Sets pin mode
@param (uint8_t) pin number
@param (uint8_t) pin mode
@return (void)
*/
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

/**
Writes a HIGH or LOW value to a digital pin
@param (uint8_t) pin number
@param (uint8_t) input, should be 1 or 0
@return (void)
*/
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

/**
Reads the value of a digital pin
@param (uint8_t) pin number
@return (int) value
*/
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

/**
Writes an analog value to a pin
@param (uint8_t) pin number
@param (uint16_t) value to write
@return (void)
*/
void analogWrite(uint8_t pinNum, uint16_t input)
{
    //  TODO: make sure pin is also analog pin

    //  ensures valid pin number and input is between 0 and 255
    if (isValidPin(pinNum) && !(input < 0 || input > 255))
    {
        uint8_t numArgs = 3;
        uint16_t command[3] = {ANALOGWRITE << 8 | numArgs, pinNum << 8 | input >> 8, input << 8};
        modbus_write_registers(arduino, 0, 3, command);
    }

    return;
}

/**
Reads the value of an analog pin
@param (uint8_t) address
@param (uint8_t) register
@param (uint8_t) number of bytes to read
@return (uint8_t*) pointer to the data that was read
*/
int analogRead(uint16_t pinNum)
{
    if (isValidPin(pinNum))
    {
        uint8_t numArgs = 1;

        //  tell the arduino we're reading a value
        uint16_t command[2] = {ANALOGREAD << 8 | numArgs, pinNum << 8};
        modbus_write_registers(arduino, 0, 2, command);

        //  read the value that should now be in address 2
        uint16_t dest;
        modbus_read_registers(arduino, 1, 1, &dest);
        return dest;
    }

    //  return -1 if invalid pin
    return -1;
}

/**
Attach servo to a pin
@param (uint16_t) pin number
@return (void)
*/
void servoAttach(uint16_t pinNum)
{
    uint8_t numArgs = 1;
    uint16_t command[2] = {SERVOATTACH << 8 | numArgs, pinNum << 8};
    modbus_write_registers(arduino, 0, 2, command);
    return;
}

/**
Detach servo from a pin
@param (uint16_t) pin number
@return (void)
*/
void servoDetach(uint16_t pinNum)
{
    uint8_t numArgs = 1;
    uint16_t command[2] = {SERVODETACH << 8 | numArgs, pinNum << 8};
    modbus_write_registers(arduino, 0, 2, command);
    return;
}

/**
Write values to a servo assigned pin
@param (uint16_t) pin number
@param (uint16_t) angle value to be written
@return (void)
*/
void servoWrite(uint16_t pinNum, uint16_t input)
{
    //  ensures valid pin number and input is between 0 and 180
    if (isValidPin(pinNum) && !(input < 0 || input > 180))
    {
        uint8_t numArgs = 2;
        uint16_t command[2] = {SERVOWRITE << 8 | numArgs, pinNum << 8 | input};
        modbus_write_registers(arduino, 0, 2, command);
    }
    return;
}

/**
Reads the last value written to a servo
@param (uint16_t) pin number
@return (int) the last value written to the servo
*/
int servoRead(uint16_t pinNum)
{
    if (isValidPin(pinNum))
    {
        uint8_t numArgs = 1;

        //  tell the arduino we're reading a value
        uint16_t command[2] = {SERVOREAD << 8 | numArgs, pinNum << 8};
        modbus_write_registers(arduino, 0, 2, command);

        //  read the value that should now be in address 2
        uint16_t dest;
        modbus_read_registers(arduino, 1, 1, &dest);

        int value = dest >> 8;

        return value;
    }

    //  return -1 if invalid pin
    return -1;
}

/**
Begins an I2C connection
@return (void)
*/
void wireBegin() {
	uint16_t command[1] = {WIREBEGIN << 8 | 0};
	modbus_write_registers(arduino, 0, 1, command);
}

/**
Terminates an I2C connection
@return (void)
*/
void wireEnd() {
	uint16_t command[1] = {WIREEND << 8 | 0};
	modbus_write_registers(arduino, 0, 1, command);
}

/**
Sets I2C clock speed
@param (int) clock speed
@return (void)
*/
void wireSetClock(uint32_t clock_speed) {
	uint16_t command[3] = {WIRECLOCK << 8 | 4, clock_speed >> 16, clock_speed & 0x0000ffff};
	modbus_write_registers(arduino, 0, 3, command);
}

/**
Writes data over an I2C connection
@param (uint8_t) address
@param (uint8_t) register
@param (uint8_t) number of bytes to be written
@param (uint8_t) data being written
@return (void)
*/
void wireWrite(uint8_t addr, uint8_t reg, uint8_t num_bytes, uint8_t *data) {
	uint16_t *command = malloc(sizeof(uint16_t) * (2 + (num_bytes+1)/2));
	command[0] = WIREWRITE << 8 | 2 + num_bytes;
	command[1] = addr << 8 | reg;
	for (int i = 0; i < num_bytes; i++) {
		if(i % 2 == 0) {
			command[2+i] = data[i] << 8;
		}
		else {
			command[2+i] |= data[i];
		}
	}
	modbus_write_registers(arduino, 0, 2 + (num_bytes+1)/2, command);
}

/**
Reads data from an I2C connection
@param (uint8_t) address
@param (uint8_t) register
@param (uint8_t) number of bytes to read
@return (uint8_t*) pointer to the data that was read
*/
uint8_t* wireRead(uint8_t addr, uint8_t reg, int num_bytes) {
	uint16_t command[3] = {WIREREAD << 8 | 3, addr << 8 | reg, num_bytes << 8};
	modbus_write_registers(arduino, 0, 3, command);

	uint16_t *registers = malloc(sizeof(uint16_t) * (num_bytes+1)/2);
	modbus_read_registers(arduino, 1, (num_bytes+1)/2, registers);
	
	uint8_t *result = malloc(sizeof(uint8_t) * num_bytes);
	for(int i = 0; i < num_bytes; i++) {
		if(i % 2 == 0) {
			result[i] = registers[i] >> 8;
		}
		else {
			result[i] = registers[i] & 0x00ff;
		}
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

void spiSettings(uint32_t speed, uint8_t order, uint8_t mode) {
	uint16_t command[4] = {SPISETTINGS << 8 | 6, speed >> 16, speed & 0x0000ffff, order << 8 | mode};
	modbus_write_registers(arduino, 0, 4, command);
}

void spiEnd() {
	uint16_t command[1] = {SPIEND << 8};
	modbus_write_registers(arduino, 0, 1, command);
}

