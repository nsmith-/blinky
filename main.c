#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "blinky.h"
#define DELAY 5

extern void pushled(void);


#define WIND_VARIABILITY 4
#define FLAME_AGILITY 5
#define WIND_CALMNESS_CONSTANT 2
#define WIND_BASELINE 30
unsigned char flame, flameprime, i, wind, x, a, b, c;
unsigned char random() {
  x++; //x is incremented every round and is not affected by any other variable
  a = (a ^ c ^ x); //note the mix of addition and XOR
  b = (b + a); //And the use of very few instructions
  c = (c + ((b >> 1) ^ a)); // the AES S-Box Operation ensures an even distributon of entropy
  return (c);
}

void simpleFlame() {
  i = i*19+51;
  grb_buffer[1] = i;
  i = i*19+51;
  grb_buffer[4] = i;
  _delay_ms(100);
  pushled();
}

void rgbFade() {
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

void fancyFlame() {
  // Thanks to https://github.com/EternityForest/CandleFlickerSimulator/blob/master/flicker2.X/main.c
  //We simulate a gust of wind by setting the wind var to a random value
  if (random() < WIND_VARIABILITY) {
    //Make a gust of wind less likely with two random teata because 255 is not enough resolution
    if(random() > 220)
    {
      wind = random();
    }
  }
  //The wind constantly settles towards its baseline value
  if (wind>WIND_BASELINE) {
    wind--;
  }
  //The flame constantly gets brighter till the wind knocks it down
  if (flame<255) {
    flame++;
  }
  //Depending on the wind strength and the calmnes modifer we calcuate the odds
  //of the wind knocking down the flame by setting it to random values
  if (random() < (wind>>WIND_CALMNESS_CONSTANT)) {
    flame = random();
  }
  //Real flames ook like they have inertia so we use this constant-aproach-rate filter
  //To lowpass the flame height
  if (flame > flameprime) {
    if (flameprime < (255 - FLAME_AGILITY)) {
      flameprime += FLAME_AGILITY;
    }
  } else {
    if (flameprime > (FLAME_AGILITY)) {
      flameprime -= FLAME_AGILITY;
    }
  }
  //How do we prevent jittering when the two are equal?
  //We don't. It adds to the realism.
  grb_buffer[1] = flameprime>>2;
  grb_buffer[4] = flameprime>>2;
  pushled();
  _delay_ms(DELAY);
}

int main(void) {
  // Set up registers
  DDRB |= _BV(PB0)|_BV(PB6); // Enable PB0 (rx led), pin 10
  DDRD |= _BV(PD0)|_BV(PD5);

  grb_buffer[0] = 0x00;
  grb_buffer[1] = 0x00;
  grb_buffer[2] = 0x00;
  grb_buffer[3] = 0x00;
  grb_buffer[4] = 0x00;
  grb_buffer[5] = 0x00;

  while(1) {
    fancyFlame();
  }

  return 0;
}
