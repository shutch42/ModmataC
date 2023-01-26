all:
	gcc ModmataC_button.c ModmataC.c `pkg-config --cflags --libs libmodbus`
