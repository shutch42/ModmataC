#include "ModmataC.h"
#include <time.h>

int delay(int ms) {
	clock_t start_time = clock();
	while(clock() < start_time + ms*1000);
}

int main() {
	//	start serial connection with arduino, given a port and baud rate
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
	
	// Set mode of pin 13 to OUTPUT
	pinMode(arduino, 13, OUTPUT);
	
	while(1) {
		// Turn on LED
		digitalWrite(arduino, 13, HIGH);
		delay(1000);
		
		// Turn off LED
		digitalWrite(arduino, 13, LOW);
		delay(1000);
	}
	
	// Clean out memory
	modbus_close(arduino);
	modbus_free(arduino);
}