#include "ModmataC.h"

int main() {
	//	start serial connection with arduino, given a port and baud rate
	connectArduino("/dev/ttyACM0", 115200, 1);
	
	servoAttach(6);
	delay(1000);
	
	for(int i = 0; i <= 180; i++) {
		servoWrite(6, i);
		delay(15);
	}
	for(int i = 180; i >= 0; i--) {
		servoWrite(6, i);
		delay(15);
	}

	servoDetach(6);
	closeConnection();
}
