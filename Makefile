CC=gcc
CFLAGS=-I/usr/include/SDL2
LDFLAGS=-lSDL2 -lSDL2_image

all: icsim controls 

icsim: icsim.o lib.o 
	$(CC) $(CFLAGS) -o icsim icsim.c lib.o  $(LDFLAGS)

controls: controls.o
	$(CC) $(CFLAGS) -o controls controls.c $(LDFLAGS)

lib.o:
	$(CC) lib.c


clean:
	rm -rf icsim controls icsim.o controls.o 

one:
	sudo modprobe can 
two:
	sudo modprobe vcan 
three:
	sudo ip link add dev vcan0 type vcan 
four:
	sudo ip link set up vcan0
