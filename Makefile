install:
	sudo apt-get install libmodbus-dev
	gcc -fPIC -c ModmataC.c -lm `pkg-config --cflags --libs libmodbus`
	gcc -shared -o libModmataC.so ModmataC.o 
	sudo mv libModmataC.so /usr/lib
	sudo cp ModmataC.h /usr/include
	rm ModmataC.o

uninstall:
	sudo rm /usr/lib/libModmataC.so
	sudo rm /usr/include/ModmataC.h
