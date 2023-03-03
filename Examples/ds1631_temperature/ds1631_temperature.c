#include <math.h>
#include "../../ModmataC.h"
#define THERMO_ADDR 0X4D

int main() {
	// start serial connection with arduino, given a port, baud rate, and id
	connectArduino("/dev/ttyACM0", 9600, 1);
	printf("Arduino connected\n");
	
	wireBegin();

	// Read in initial config settings
	uint8_t *config = wireRead(THERMO_ADDR, 0xAC, 1);
	// Modify config settings
	config[1] |= 0x0D;
	wireWrite(THERMO_ADDR, 0xAC, 1, config);
	
	while(1) {
		// Start temperature conversion
		wireWrite(THERMO_ADDR, 0x51, 0, 0);

		// Read in temperature bytes
		uint8_t *bytes = wireRead(THERMO_ADDR, 0xAA, 2);
		// printf("%02x %02x\n", bytes[0], bytes[1]);

		// Convert bytes to temperature number
		double temperature_c = 0;
		temperature_c += bytes[0];
		for(int i = 0; i < 4; i++) {
			temperature_c += ((bytes[1] >> 4+i) & 0x01) / pow(2, i+1);
		}
		printf("%.04f F\n", temperature_c * 9/5.0 + 32);

		// Wait a second until the next sample
		free(bytes);
		delay(1000);
	}
	closeConnection();
	return 0;
}

