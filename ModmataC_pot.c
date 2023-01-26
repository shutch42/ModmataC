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
	
	int value = -1;
    int led_val = 0;
    pinMode(arduino, 13, OUTPUT);

	while(1) {
		value = analogRead(arduino, 18) / 4;
        if(value != led_val) {
            led_val = value;
            analogWrite(arduino, 13, led_val);
        }
        // if(value && !led_state) {
        //     led_state = HIGH;
        //     digitalWrite(arduino, 13, HIGH);
        // }
        // else if(!value && led_state) {
        //     led_state = LOW;
        //     digitalWrite(arduino, 13, LOW);
        // }
        printf("%i\n", value);
	}
	
	// Clean out memory
	modbus_close(arduino);
	modbus_free(arduino);
}