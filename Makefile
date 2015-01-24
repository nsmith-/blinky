# Configuration
MCU=atmega32u4

# Headers
HEADERS=blinky.h

# linux
#AVRDUDE_PORT=/dev/ttyACM0
# mac
AVRDUDE_PORT=/dev/cu.usbmodem1411

AVRDUDE_PROGRAMMERID=avr109

# Less often changed stuff
CC=avr-gcc
CFLAGS=-I. -O2 -Wall -mmcu=$(MCU)
ASMFLAGS=-I. -x assembler-with-cpp -mmcu=$(MCU)
LFLAGS=-lm

# Rules

.SUFFIXES: .c .S .out .hex
.PHONY: install clean

all: blinky.out
	avr-objdump -h $^

blinky.out: main.o pushled.o
	$(CC) $(LFLAGS) -o $@ $^

install: blinky.hex
	while [ ! -w $(AVRDUDE_PORT) ]; do sleep 1; done;
	avrdude -c $(AVRDUDE_PROGRAMMERID) \
		-p $(MCU) -P $(AVRDUDE_PORT) -b57600 -D \
		-U flash:w:$^:i

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S $(HEADERS)
	$(CC) $(ASMFLAGS) -c $< -o $@

.out.hex:
	avr-objcopy -j .text -j .data -O ihex $< $@

clean: 
	rm -f *.o *.out

