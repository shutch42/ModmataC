all:
	gcc ModmataC_servo.c ModmataC.c `pkg-config --cflags --libs libmodbus`
