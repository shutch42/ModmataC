#include "ModmataC.h"
#include <time.h>

int delay(int ms) {
	clock_t start_time = clock();
	while(clock() < start_time + ms*1000);
}

int main() {
	//	start serial connection with arduino, given a port and baud rate
	modbus_t *arduino = connectArduino("/dev/ttyACM0", 9600, 1);
	
	servoAttach(arduino, 9);
	
	while(1) {
		servoWrite(arduino, 0);
		delay(1000);
		
		servoWrite(arduino, 180);
		delay(1000);
	}
	
	// Clean out memory
	modbus_close(arduino);
	modbus_free(arduino);
}
