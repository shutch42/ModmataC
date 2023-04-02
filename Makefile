install:
	cd libmodbus && ./autogen.sh
	cd libmodbus && ./configure
	cd libmodbus/src && make install
	gcc -fPIC -c ModmataC.c
	gcc -shared -o libModmataC.so ModmataC.o -lmodbus
	mv libModmataC.so /usr/lib
	cp ModmataC.h /usr/include
	rm ModmataC.o

uninstall:
	cd libmodbus/src && make uninstall
	cd libmodbus/src && make clean
	cd libmodbus && make uninstall
	cd libmodbus && make clean
	rm /usr/lib/libModmataC.so
	rm /usr/include/ModmataC.h
