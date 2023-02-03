#include "ModmataC.h"
#include <time.h>

int delay(int ms) {
	clock_t start_time = clock();
	while(clock() < start_time + ms*1000);
}

int main() {
	//	start serial connection with arduino, given a port and baud rate
	connectArduino("/dev/ttyACM0", 9600, 1);

	// Set mode of pin 13 to OUTPUT
	pinMode(13, OUTPUT);
	
	while(1) {
		// Turn on LED
		digitalWrite(13, HIGH);
		delay(1000);
		
		// Turn off LED
		digitalWrite(13, LOW);
		delay(1000);
	}
}
