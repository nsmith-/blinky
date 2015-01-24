#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "blinky.h"
#define DELAY 5

volatile char grb[GRBSIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char * grbaddr = grb;
extern void pushled(void);

int main(void)
{
  // Set up registers
  DDRB |= _BV(PB0)|_BV(PB6); // Enable PB0 (rx led), pin 10
  DDRD |= _BV(PD0)|_BV(PD5);
  //sei();

  //PORTB |= _BV(PB6);
  //pushled(); // takes 33us
  //PORTB &= ~_BV(PB6);

  // uint8_t failed miserably?!
  int i;
  while(1) {
    for (i=0; i<=255; i++) {
      grb[1] = i;
      grb[4] = i;
      _delay_ms(DELAY);
      pushled();
    }
    for (i=0; i<=255; i++) {
      grb[0] = i; grb[1] = 0xff-i;
      grb[3] = i; grb[4] = 0xff-i;
      _delay_ms(DELAY);
      pushled();
    }
    for (i=0; i<=255; i++) {
      grb[2] = i; grb[0] = 0xff-i;
      grb[5] = i; grb[3] = 0xff-i;
      _delay_ms(DELAY);
      pushled();
    }
    for (i=0; i<=255; i++) {
      grb[2] = 0xff-i;
      grb[5] = 0xff-i;
      _delay_ms(DELAY);
      pushled();
    }
  }

  return 0;
}
