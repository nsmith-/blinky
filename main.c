#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "blinky.h"
#define DELAY 5

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
      grb_buffer[1] = i;
      grb_buffer[4] = i;
      _delay_ms(DELAY);
      pushled();
    }
    for (i=0; i<=255; i++) {
      grb_buffer[0] = i; grb_buffer[1] = 0xff-i;
      grb_buffer[3] = i; grb_buffer[4] = 0xff-i;
      _delay_ms(DELAY);
      pushled();
    }
    for (i=0; i<=255; i++) {
      grb_buffer[2] = i; grb_buffer[0] = 0xff-i;
      grb_buffer[5] = i; grb_buffer[3] = 0xff-i;
      _delay_ms(DELAY);
      pushled();
    }
    for (i=0; i<=255; i++) {
      grb_buffer[2] = 0xff-i;
      grb_buffer[5] = 0xff-i;
      _delay_ms(DELAY);
      pushled();
    }
  }

  return 0;
}
