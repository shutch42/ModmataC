#include <ModmataC.h>

int main() {
	connectArduino("/dev/ttyACM0", 115200, 1);
		
	pinMode(7, INPUT);
    	pinMode(13, OUTPUT);
	int value = -1;
    	int led_state = LOW;
	
	while(1) {
		value = digitalRead(7);
        	if(value && !led_state) {
            		led_state = HIGH;
            		digitalWrite(13, HIGH);
        	}
        	else if(!value && led_state) {
            		led_state = LOW;
            		digitalWrite(13, LOW);
        	}
        	printf("%i\n", value);
	}
	
}
