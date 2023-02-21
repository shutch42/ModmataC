all:
	gcc Examples_ModmataC/ModmataC_servo.c ModmataC.c `pkg-config --cflags --libs libmodbus`
