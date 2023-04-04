#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "parser.h"
#include "signal.h"
#include "arduino_functions.h"

pthread_t arduino_thread;

void run_fn(void* fn()) {
	stop();
	pthread_join(arduino_thread, NULL);
	run();
	pthread_create(&arduino_thread, NULL, fn, NULL);
}

void run_fn_arg_int(void* fn(), int angle) {
	int* angle_ptr = malloc(sizeof(int));
	*angle_ptr = angle;
	stop();
	pthread_join(arduino_thread, NULL);
	run();
	pthread_create(&arduino_thread, NULL, fn, (void *)angle_ptr);
}

void run_fn_arg_str(void* fn(), char* msg) {
	stop();
	pthread_join(arduino_thread, NULL);
	run();
	pthread_create(&arduino_thread, NULL, fn, (void *)msg);
}

int main() {
	char request[80];
	// pthread_create(&arduino_thread, NULL, blink, NULL);
	while(1) {
		fgets(request, 80, stdin);
		request[strlen(request)-1] = '\0'; // Remove newline
		struct token_info tokens = parse(request, ' ');
		
		if(!strcmp(request, "HELP")) {
			printf("Command list:\n");
			printf("\tBLINK\n");
			printf("\tBUTTON\n");
			printf("\tDIGITAL_ANALOG\n");
			printf("\tFADE\n");
			printf("\tSERVO <angle>\n");
			printf("\tSERVO_CONTROL\n");
			printf("\tTEMP\n");
			printf("\tRADIO <message>\n");
			printf("\tLCD <message>\n");
			printf("\tSTOP\n");
			printf("\tEXIT\n");
		}
		else if(!strcmp(request, "BLINK")) {
			run_fn(blink);
		}
		else if(!strcmp(request, "BUTTON")) {
			run_fn(button);
		}
		else if(!strcmp(request, "DIGITAL_ANALOG")) {
			run_fn(digital_analog);
		}
		else if(!strcmp(request, "FADE")) {
			run_fn(fade);
		}
		else if(tokens.count == 2 && !strcmp(tokens.list[0], "SERVO")) {
			run_fn_arg_int(servo, atoi(tokens.list[1]));
		}
		else if(!strcmp(request, "SERVO_CONTROL")) {
			run_fn(servo_control);
		}
		else if(!strcmp(request, "TEMP")) {
			run_fn(temperature);
		}
		else if(tokens.count >=2 && !strcmp(tokens.list[0], "RADIO")) {
			char radio_msg[80] = {'\0'};
			for(int i = 1; i < tokens.count; i++) {
				strcat(radio_msg, tokens.list[i]);
				strcat(radio_msg, " ");
			}
			run_fn_arg_str(radio, radio_msg);
		}
		else if(tokens.count >= 2 && !strcmp(tokens.list[0], "LCD")) {
			char lcd_msg[80] = {'\0'};
			for(int i = 1; i < tokens.count; i++) {
				strcat(lcd_msg, tokens.list[i]);
				strcat(lcd_msg, " ");
			}
			run_fn_arg_str(lcd, lcd_msg);
		}
		else if(!strcmp(request, "STOP")) {
			stop();
			pthread_join(arduino_thread, NULL);
		}
		else if(!strcmp(request, "EXIT")) {
			stop();
			pthread_join(arduino_thread, NULL);
			return 0;
		}
		else {
			printf("Error: Command not recognized\n");
		}

	}
}
