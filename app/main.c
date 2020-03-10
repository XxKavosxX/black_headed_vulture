/*
    main.c
    Author:
       Maike Rodrigo A. Reis
       Electrical Engineering Stundent at University Federal of Pará
       maikerdralcantara@gmail.com
    Created on:
       06 Mar 2020
*/

#define __AVR_ATmega328P__
#define F_CPU 16000000UL // or whatever may be your frequency
#define __DELAY_BACKWARD_COMPATIBLE__
#define __DELAY_ROUND_DOWN__

#include <avr/io.h>
#include <util/delay.h>
#include "../src/libs/adc.h"

int main(void)
{
    DDRB = 0xFF; // initialize port C
    while (1)
    {   
        
        sum();
        PORTB ^= 1 << PINB5; // PC0 = High = Vcc
        _delay_ms(100);      // wait 500 milliseconds

        //LED off
        PORTB &= 0 << PINB5; // PC0 = Low = 0v
        _delay_ms(100);      // wait 500 milliseconds
    }
}