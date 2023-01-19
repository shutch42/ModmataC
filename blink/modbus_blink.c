#include <stdio.h>
#include <modbus.h>
#include <errno.h>
#include <time.h>

#define PINMODE 1
#define DIGITALWRITE 2

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

int delay(int ms) {
	clock_t start_time = clock();
	while(clock() < start_time + ms*1000);
}

int main() {
	// Connect arduino to port /dev/ttyACM0 with baud rate 9600, no parity bits, 8 data bits, and 1 stop bit
	modbus_t *arduino;
	arduino = modbus_new_rtu("/dev/ttyACM0", 9600, 'N', 8, 1);
	
	if(arduino == NULL) {
		fprintf(stderr, "Unable to create the libmodbus context\n");
		return -1;
	}
	
	// Set the device id to communicate with
	if(modbus_set_slave(arduino, 1) == -1) {
		fprintf(stderr, "Invalid slave ID\n");
		modbus_free(arduino);
		return -1;
	}
	
	// Test the connection
	if(modbus_connect(arduino) == -1) {
		fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		modbus_free(arduino);
		return -1;
	}
	
	// Set pinMode of pin 13 to OUTPUT
	uint16_t command[3] = {PINMODE, 13, OUTPUT};
	modbus_write_registers(arduino, 0, 3, command);
	
	while(1) {
		// Turn on LED
		command[0] = DIGITALWRITE;
		command[1] = 13;
		command[2] = HIGH;
		modbus_write_registers(arduino, 0, 3, command);
		delay(1000);
		
		// Turn off LED
		command[0] = DIGITALWRITE;
		command[1] = 13;
		command[2] = LOW;
		modbus_write_registers(arduino, 0, 3, command);
		delay(1000);
	}
	
	// Clean out memory
	modbus_close(arduino);
	modbus_free(arduino);
}
