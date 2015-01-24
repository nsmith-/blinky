#include <avr/io.h>
#include "blinky.h"

.extern grbaddr  

.global pushled 
pushled:
  ldi r18, _BV(COM0B1)|_BV(WGM01)|_BV(WGM00)
  out _SFR_IO_ADDR(TCCR0A), r18
  ldi r18, _BV(WGM02)
  out _SFR_IO_ADDR(TCCR0B), r18
  ldi r18, 19; ; for 800kHz period
  out _SFR_IO_ADDR(OCR0A), r18
  ; 1: OCR0B = 10 for .375us
  ; 0: OCR0B = 5  for .6875us
  ; Reset >50us

  ; Load grb address into Z (r30:r31)
  lds    r31, grbaddr+1
  lds    r30, grbaddr 

  ; byte sent counter
  ldi    r21, lo8(GRBSIZE+1)

loop:
  ; check if done sending (normally 2c)
  dec r21
  breq done
  ; set up position counter (1c)
  ldi    r18, 8
  ; Load current byte (2c)
  ld  r19, z+
byte:
  ; Shift left (msb becomes carry, 1c)
  lsl  r19
  ; branch if carry set (1c, 2 if true)
  brcs  send1
  ; Set OCR0B to 5 cycles (1c)
  ldi    r20, 5
  ; skip send1 (2c)
  brcc  send
send1:
  ; Set OCR0B to 10 cycles (1c)
  ldi    r20, 10
  ; sync with sending 0 (1c)
  nop
send:
  ; Push OCR0B value (1c)
  out _SFR_IO_ADDR(OCR0B), r20
  ; pad to 20 clock cycles (4c)
  nop
  nop
  nop
  nop
  ; start (or continue) timer (2c)
  ldi  r22, _BV(CS00)|_BV(WGM02)
  out _SFR_IO_ADDR(TCCR0B), r22
  ; decrement bit counter (1c)
  dec  r18 
  ; test if counter==0 (1c, 2 if true)
  breq loop  
  ; sync
  nop
  nop
  nop
  nop
  ; go to next bit (2c)
  brne byte

done:
  ; we are 6 cycles from starting timer
  ; buffer to at least 15 to safely transmit last bit
  ldi r21, 0x04
buf:
  dec r21
  brne buf
  ; stop timer
  ldi  r22, _BV(WGM02)
  out _SFR_IO_ADDR(TCCR0B), r22
  ret
