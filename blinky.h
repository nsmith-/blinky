#ifndef BLINKY_H
#define BLINKY_H

#define GRBSIZE 6

#ifndef PUSHLED_ASM
char grb_buffer[GRBSIZE];
const char * grbaddr = grb_buffer;
#endif

#endif //BLINKY_H

