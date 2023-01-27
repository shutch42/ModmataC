#include "ModmataC.h"
#include <time.h>

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
	
	pinMode(arduino, 7, INPUT);
    pinMode(arduino, 13, OUTPUT);
	int value = -1;
    int led_state = LOW;

	while(1) {
		value = digitalRead(arduino, 7);
        if(value && !led_state) {
            led_state = HIGH;
            digitalWrite(arduino, 13, HIGH);
        }
        else if(!value && led_state) {
            led_state = LOW;
            digitalWrite(arduino, 13, LOW);
        }
        printf("%i\n", value);
	}
	
	// Clean out memory
	modbus_close(arduino);
	modbus_free(arduino);
}
