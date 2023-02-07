#include "ModmataC.h"
#include <time.h>
#include <stdlib.h>

int delay(int ms) {
	clock_t start_time = clock();
	while(clock() < start_time + ms*1000);
}

int main() {
	//	start serial connection with arduino, given a port and baud rate
	modbus_t *arduino = connectArduino("/dev/ttyACM0", 9600, 1);
	
	servoAttach(arduino, 6);
	delay(1000);
	
	/* servoWrite(arduino, 6, 180);
	delay(1000);
	servoWrite(arduino, 6, 0);
	delay(1000);
	servoDetach(arduino, 6);
	*/
	// while(1) {
		for(int i = 0; i <= 180; i++) {
			servoWrite(arduino, 6, i);
			delay(15);
		}
		for(int i = 180; i >= 0; i--) {
			servoWrite(arduino, 6, i);
			delay(15);
		}
	// }

	servoDetach(arduino, 6);
	// Clean out memory
	modbus_close(arduino);
	modbus_free(arduino);
}
