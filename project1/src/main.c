#ifndef MAIN_C
#define MAIN_C

#include "avr.h"

#define LED 0
#define BUTTON 1


int main(void)
{
  DDRB = 3; // set B = 00000011, meaning ___ and ___ set to output
  DDRB &= ~(1<<BUTTON); // set 
  PORTB = 2; // set LED to 0 and BUTTON to 1 
  while(1)
  {
    if(!GET_BIT(PINB, BUTTON))//!(PINB & (1<<BUTTON))
    {
      SET_BIT(PORTB, LED);
      avr_wait(500);
      CLR_BIT(PORTB, LED);
      avr_wait(500);
    }
  }
  return 0;
}

#endif