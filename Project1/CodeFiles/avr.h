#ifndef _AVR_H_
#define _AVR_H_

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

// defined frequency of the crystal oscillator 8MHz
#define XTAL_FRQ 8000000lu

// (p=registor, i=bit index)
#define SET_BIT(p,i) ((p) |=  (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) &   (1 << (i)))

// inserts an assembly instruction to perform a no-operation for timing purposes
#define NOP() asm volatile("nop"::)

void avr_wait(unsigned short msec); //unsigned short msec

#endif /* _AVR_H_ */