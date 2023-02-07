all:
	gcc ModmataC_blink.c ModmataC.c `pkg-config --cflags --libs libmodbus`
