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
#include "adc.h"

int main(void)
{
    DDRB = 0xFF; // initialize port C
    while (1)
    {   
        adc_get_status();
        PORTB ^= 1 << PINB5; // PC0 = High = Vcc
        _delay_ms(500);      // wait 500 milliseconds
    }
}