#include <ModmataC.h>
#include <string.h>

#define LCD_SETCURSOR 20
#define LCD_PRINT 21
#define LCD_CLEAR 22

void lcdSetCursor(int row, int col) {
	uint8_t position[2] = {row, col};
	transmitRegisters(LCD_SETCURSOR, 2, position);
}

void lcdPrint(char* print_str) {
	transmitRegisters(LCD_PRINT, strlen(print_str)-1, print_str);
}

void lcdClear() {
	transmitRegisters(LCD_CLEAR, 0, NULL);
}

int main() {
	connectArduino("/dev/ttyACM0", 115200, 1);	
	
	while(1) {
		char str[80];
		fgets(str, 80, stdin);
		lcdClear();
		lcdPrint(str);
	}

	closeConnection();
	return 0;
}
