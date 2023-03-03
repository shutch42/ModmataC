#include "../../ModmataC.h"

int main() {
	//	start serial connection with arduino, given a port and baud rate
	connectArduino("/dev/ttyACM0", 9600, 1);

	int value = -1;
	int led_val = 0;
	pinMode(13, OUTPUT);

	while(1) {
		value = analogRead(18) / 4;
		if(value != led_val) {
            		led_val = value;
            		analogWrite(13, led_val);
        	}
                printf("%i\n", value);
	}	
}
