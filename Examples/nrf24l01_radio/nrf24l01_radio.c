#include "../../ModmataC.h"
#include <ncurses.h>

#define IRQ 8
#define CE 9
#define CS 10

void printReg(char* name, uint8_t reg, int num_bytes) {
	uint8_t *write_buf = malloc(sizeof(uint8_t) * (num_bytes + 1));
	write_buf[0] = reg;
	for(int i = 0; i < num_bytes; i++) {
		write_buf[i+1] = 0xAA;
	}
	uint8_t *read_buf = spiTransferBuf(CS, write_buf, num_bytes + 1);
	
	printw("%s: ", name);
	for(int i = 0; i < num_bytes; i++) {
		printw("%02x ", read_buf[i+1]);
	}
	printw("\n");

	free(write_buf);
	free(read_buf);
}

void setTransmitMode(uint8_t my_addr, uint8_t their_addr, int debug) {
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

	if(debug) {
		printReg("SETUP_RETR", 0x04, 1);
		printReg("RF_SETUP", 0x06, 1);
		printReg("RF_PW_P0", 0x11, 1);
		printReg("RF_PW_P1", 0x12, 1);
		printReg("EN_AA", 0x01, 1);
		printReg("EN_RXADDR", 0x02, 1);
		printReg("CONFIG", 0x00, 1);
		printReg("RF_CH", 0x05, 1);
		printReg("TX_ADDR", 0x10, 5);
		printReg("RX_ADDR_P0", 0x0a, 5);
		printReg("RX_ADDR_P1", 0x0b, 5);
		printReg("STATUS", 0x07, 1);
	}
}

void setReceiveMode(uint8_t my_addr, int debug) {
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
	// Set receive mode and 2-byte CRC's
	write_buf[0] = 0x00 | 0x20;
	write_buf[1] = 0x0f;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// RF_CH Register
	// Set frequency channel to 2.476 GHz
	write_buf[0] = 0x05 | 0x20;
	write_buf[1] = 0x4c;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	// TX_ADDR Register
	// Set to own address
	write_buf[0] = 0x10 | 0x20;
	for (int i = 1; i < 6; i++) {
		write_buf[i] = my_addr;
	}
	read_buf = spiTransferBuf(CS, write_buf, 6);
	free(read_buf);

	// RX_ADDR_P0 Register
	// Set to own address
	write_buf[0] = 0x0a | 0x20;
	read_buf = spiTransferBuf(CS, write_buf, 6);
	free(read_buf);

	// RX_ADDR_P1 Register
	// Set to own address
	write_buf[0] = 0x0b | 0x20;
	read_buf = spiTransferBuf(CS, write_buf, 6);
	free(read_buf);

	// STATUS
	// Clear status register
	write_buf[0] = 0x07 | 0x20;
	write_buf[1] = 0x7e;
	read_buf = spiTransferBuf(CS, write_buf, 2);
	free(read_buf);

	if (debug) {
		printReg("SETUP_RETR", 0x04, 1);
		printReg("RF_SETUP", 0x06, 1);
		printReg("RF_PW_P0", 0x11, 1);
		printReg("RF_PW_P1", 0x12, 1);
		printReg("EN_AA", 0x01, 1);
		printReg("EN_RXADDR", 0x02, 1);
		printReg("CONFIG", 0x00, 1);
		printReg("RF_CH", 0x05, 1);
		printReg("TX_ADDR", 0x10, 5);
		printReg("RX_ADDR_P0", 0x0a, 5);
		printReg("RX_ADDR_P1", 0x0b, 5);
		printReg("STATUS", 0x7e, 1);
	}
}

int main() {
	// start serial connection with arduino, given a port, baud rate, and id
	connectArduino("/dev/ttyACM0", 9600, 1);
	
	// Set up chip enable pins for SPI
	pinMode(CS, OUTPUT);
	pinMode(CE, OUTPUT);
	pinMode(IRQ, INPUT);
	digitalWrite(CS, HIGH);
	digitalWrite(CE, HIGH);
	
	// Start up SPI library
	spiBegin();

	// Start ncurses
	initscr();
	scrollok(stdscr, 1);
	
	// Set addresses for users
	int my_addr = -1;
	printw("Choose your device address (0-255): ");
	scanw("%d", &my_addr);
	while (my_addr < 0 || my_addr > 255) {
		printw("Invalid address!\n");
		printw("Choose your device address (0-255): ");
		scanw("%d", &my_addr);
	}
	
	int their_addr = -1;
	printw("Enter a device address to communicate with (0-255): ");
	scanw("%d", &their_addr);
	while(their_addr < 0 || their_addr > 255) {
		printw("Invalid address!\n");
		printw("Enter a device address to communicate with (0-255): ");
		scanw("%d", &their_addr);
	}

	// Power on the nrf24
	digitalWrite(CE, LOW);
	delay(100);
	
	// Set all the registers to receive messages
	setReceiveMode(my_addr, 0);
	
	// Enter receive mode
	digitalWrite(CE, HIGH);
	delayMicroseconds(140);
	
	
	char str[33] = {0x00};
	// char *s = str;
	// size_t str_size = 32;
	char c;
	while(1) {
		// printReg("STATUS", 0x07, 1);
		nodelay(stdscr, 1);
		c = getch();
		if (c == 0x0A) {
			nodelay(stdscr, 0);
			printw("> ");
			getnstr(str, 32);
			
			// Switch to transmit mode
			digitalWrite(CE, LOW);
			setTransmitMode(my_addr, their_addr, 0);
			digitalWrite(CE, HIGH);
			delayMicroseconds(140);
			
			// Set up buffers for byte transfers
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
			for (int i = 0; str[i] != 0x00; i++) { // This should work right?
				wr_buf[i+1] = str[i];
			}
			read_buf = spiTransferBuf(CS, wr_buf, 33);
			free(read_buf);
			
			// Wait for message to send (IRQ goes low)
			while(digitalRead(IRQ));
			
			/*
			// Check for ACK from receiver
			wr_buf[0] = 0x07;
			read_buf = spiTransferBuf(CS, wr_buf, 2);
			if((read_buf[1] & 0x20) == 0x20) {
				// printf("Message sent!\n");
			}
			else if ((read_buf[1] & 0x10) == 0x10) {
				printw("No ACK received :(\n");
			}
			else {
				printw("Unknown error: %02x\n", read_buf[1]);
			}
			free(read_buf);
			*/

			// Clear TX FIFO again
			wr_buf[0] = 0xE1;
			read_buf = spiTransferBuf(CS, wr_buf, 1);
			free(read_buf);

			// Switch back to receive mode
			digitalWrite(CE, LOW);
			setReceiveMode(my_addr, 0);
			digitalWrite(CE, HIGH);
			delayMicroseconds(140);
		}
			
		// Check for received message
		if (!digitalRead(IRQ)) {
			// printf("Message available!\n");
			uint8_t write_buf[33];
			uint8_t *read_buf;
			
			// Flush TX FIFO
			write_buf[0] = 0xE1;
			read_buf = spiTransferBuf(CS, write_buf, 1);
			free(read_buf);

			// R_RX_PAYLOAD
			printw("< ");
			write_buf[0] = 0x61;
			read_buf = spiTransferBuf(CS, write_buf, 33);
			for (int i = 0; i < 32; i++) {
				printw("%c", read_buf[i+1]);
			}
			printw("\n");
			free(read_buf);

			// Flush RX FIFO
			write_buf[0] = 0xE2;
			read_buf = spiTransferBuf(CS, write_buf, 1);
			free(read_buf);

			// Clear STATUS Register
			write_buf[0] = 0x07 | 0x20;
			write_buf[1] = 0x7E;
			read_buf = spiTransferBuf(CS, write_buf, 2);
			free(read_buf);
		}
	}
	
	// Close ncurses window	
	endwin();

	// Disconnect modmata
	closeConnection();	
}

