#ifndef BLINKY_H
#define BLINKY_H

#define GRBSIZE 6

#ifndef PUSHLED_ASM
volatile char grb_buffer[GRBSIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
#endif

#endif //BLINKY_H

