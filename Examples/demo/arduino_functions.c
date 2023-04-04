#include "arduino_functions.h"

void lcdSetCursor(int row, int col) {
	uint8_t position[2] = {row, col};
	transmitRegisters(LCD_SETCURSOR, 2, position);
}

void lcdPrint(char* print_str) {
	transmitRegisters(LCD_PRINT, strlen(print_str), print_str);
}

void lcdClear() {
	uint8_t dummy = 0;
	transmitRegisters(LCD_CLEAR, 0, &dummy);
}

void* blink() {
	// start serial connection with arduino, given a port, baud rate, and id
	connectArduino("/dev/ttyACM0", 115200, 1);

	// Set mode of pin 13 to OUTPUT
	pinMode(13, OUTPUT);
	
	while(is_running()) {
		// Turn on LED
		digitalWrite(13, HIGH);
		delay(1000);
		
		// Turn off LED
		digitalWrite(13, LOW);
		delay(1000);
	}

	digitalWrite(13, LOW);
	closeConnection();
}

void* button() {
	connectArduino("/dev/ttyACM0", 115200, 1);

	pinMode(13, OUTPUT);
	pinMode(1, INPUT);
	int value = -1;
	int led_state = LOW;

	while(is_running()) {
		value = digitalRead(1);
		if(value && !led_state) {
			led_state = HIGH;
			digitalWrite(13, HIGH);
			printf("Button pressed\n");
		}
		else if(!value && led_state) {
			led_state = LOW;
			digitalWrite(13, LOW);
			printf("Button released\n");
		}
	}
	
	digitalWrite(13, LOW);
	closeConnection();
}

void* digital_analog() {
	connectArduino("/dev/ttyACM0", 115200, 1);
	pinMode(13, OUTPUT);
	pinMode(11, OUTPUT);

	int analog_val = 0;
	int up = 1;
	while(is_running()) {
		analogWrite(11, analog_val);
		if(up) {
			analog_val += 5;
			if(analog_val == 125) {
				digitalWrite(13, HIGH);
			}
			else if(analog_val >= 255) {
				up = 0;
			}
		}
		else {
			analog_val -= 5;
			if(analog_val == 125) {
				digitalWrite(13, LOW);
			}
			else if(analog_val <= 0) {
				up = 1;
			}
		}
	}

	digitalWrite(13, LOW);
	analogWrite(11, 0);
	closeConnection();
}

void* fade() {
	connectArduino("/dev/ttyACM0", 115200, 1);
	pinMode(11, OUTPUT);
	pinMode(18, INPUT);
	
	int value = 0;
	int led_val = 0;
	while(is_running()) {
		value = analogRead(18) / 4;
		if(value != led_val) {
			led_val = value;
			analogWrite(11, led_val);
		}
	}

	analogWrite(11, 0);
	closeConnection();
}

void* servo(void* angle) {
	connectArduino("/dev/ttyACM0", 115200, 1);
	servoAttach(10);

	int* angle_ptr = (int *)angle;
	printf("%d\n", *angle_ptr);
	servoWrite(10, *angle_ptr);
	delay(500);

	free(angle_ptr);
	servoDetach(10);
	closeConnection();
}

void* servo_control() {
	connectArduino("/dev/ttyACM0", 115200, 1);
	servoAttach(10);
	pinMode(18, INPUT);
	int value = 0;

	while(is_running()) {
		value = analogRead(18) / 1024.0 * 180;
		if(value >= 0 && value <= 180) {
			servoWrite(10, value);
		}
	}

	servoDetach(10);
	closeConnection();
}	

void* temperature() {
	connectArduino("/dev/ttyACM0", 115200, 1);
	
	wireBegin();
	
	// Read in initial config settings
	uint8_t *config = wireRead(THERMO_ADDR, 0xAC, 1);
	// Modify config settings
	config[1] |= 0x0D;
	wireWrite(THERMO_ADDR, 0xAC, 1, config);

	// Start temperature conversion
	wireWrite(THERMO_ADDR, 0x51, 0, 0);

	// Read in temperature bytes
	uint8_t *bytes = wireRead(THERMO_ADDR, 0xAA, 2);

	// Convert bytes to temperature number
	double temperature_c = 0;
	temperature_c += bytes[0];
	for(int i = 0; i < 4; i++) {
		temperature_c += ((bytes[1] >> 4+i) & 0x01) / pow(2, i+1);
	}
	char temp_str[17];
	sprintf(temp_str, "%.04f F", temperature_c * 9/5.0 + 32);
	printf("%s\n", temp_str);
	wireEnd();

	lcdClear();
	lcdPrint(temp_str);
	
	free(bytes);
	closeConnection();
}

void setTransmitMode(uint8_t my_addr, uint8_t their_addr) {
	uint8_t write_buf[6];
	uint8_t *read_buf;

	// SETUP_RETR Register
	// Set max delay and retransmits
	write_buf[0] = 0x04 | 0x20;
	write_buf[1] = 0xff;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// RF_SETUP Register
	// Set data rate to 250kbps at max power
	write_buf[0] = 0x06 | 0x20;
	write_buf[1] = 0x27;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// RF_PW_P0 Register
	// Set payload size on pipe 0 to 32 bytes
	write_buf[0] = 0x11 | 0x20;
	write_buf[1] = 0x20;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// RF_PW_P1 Register
	// Set payload size on pipe 1 to 32 bytes
	write_buf[0] = 0x12 | 0x20;
	write_buf[1] = 0x20;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// EN_AA Register
	// Set auto-acknowledgement on receive pipes
	write_buf[0] = 0x01 | 0x20;
	write_buf[1] = 0x03;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// EN_RXADDR Register
	// Enable receive pipes
	write_buf[0] = 0x02 | 0x20;
	write_buf[1] = 0x03;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// CONFIG Register
	// Set transmit mode and 2-byte CRC's
	write_buf[0] = 0x00 | 0x20;
	write_buf[1] = 0x0e;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// RF_CH Register
	// Set frequency channel to 2.476 GHz
	write_buf[0] = 0x05 | 0x20;
	write_buf[1] = 0x4c;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// TX_ADDR Register
	// Set to receiver address
	write_buf[0] = 0x10 | 0x20;
	for (int i = 1; i < 6; i++) {
		write_buf[i] = their_addr;
	}
	read_buf = spiTransferBuf(CS, write_buf, 6);
	free(read_buf);

	// RX_ADDR_P0 Register
	// Set to receiver address
	write_buf[0] = 0x0a | 0x20;
	read_buf = spiTransferBuf(CS, write_buf, 6);
	free(read_buf);

	// RX_ADDR_P1 Register
	// Set to own address
	write_buf[0] = 0x0b | 0x20;
	for (int i = 1; i < 6; i++) {
		write_buf[i] = my_addr;
	}
	read_buf = spiTransferBuf(CS, write_buf, 6);
	free(read_buf);

	// STATUS Register
	// Clear current status
	write_buf[0] = 0x07 | 0x20;
	write_buf[1] = 0x7e;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);
}

void* radio(void* str) {
	connectArduino("/dev/ttyACM0", 115200, 1);
	char* msg = (char *)str;
	
	pinMode(CS, OUTPUT);
	pinMode(CE, OUTPUT);
	pinMode(IRQ, INPUT);
	digitalWrite(CS, HIGH);
	digitalWrite(CE, HIGH);

	spiBegin();

	digitalWrite(CE, LOW);
	delay(100);
	setTransmitMode(0, 1);
	digitalWrite(CE, HIGH);
	delayMicroseconds(140);

	uint8_t wr_buf[33] = {0x00};
	uint8_t *read_buf;

	// Clear TX FIFO
	wr_buf[0] = 0xE1;
	read_buf = spiTransferBuf(CS, wr_buf, 1);
	free(read_buf);
			
	// Clear RX FIFO
	wr_buf[0] = 0xE2;
	read_buf = spiTransferBuf(CS, wr_buf, 1);
	free(read_buf);
			
	// Queue up the message
	wr_buf[0] = 0xA0;
	for (int i = 0; msg[i] != 0x00; i++) { // This should work right?
		wr_buf[i+1] = msg[i];
	}
	read_buf = spiTransferBuf(CS, wr_buf, 33);
	free(read_buf);
			
	// Wait for message to send (IRQ goes low)
	while(digitalRead(IRQ));
			
	// Clear TX FIFO again
	wr_buf[0] = 0xE1;
	read_buf = spiTransferBuf(CS, wr_buf, 1);
	free(read_buf);
	
	// free(msg);
	spiEnd();
	closeConnection();
}

void* lcd(void* str) {
	connectArduino("/dev/ttyACM0", 115200, 1);
	char* msg = (char *)str;
	
	lcdClear();
	lcdPrint(msg);

	closeConnection();
}
