#include "../../ModmataC.h"

int main() {
    // start serial connection with arduino, given a port and baud rate
    connectArduino("/dev/ttyACM0", 9600, 1);
   
    int value = -1;
    int led_val = 0;

    pinMode(18, INPUT);
    
    servoAttach(6);

    while(1) {
        value = analogRead(18) / 1024.0 * 180;

        if(value >= 0 && value <= 180)
        {
            servoWrite(6, value);
        }
        printf("%i\n", value);
    }
}

