/*
    soft_usart.c
    Author:
       Maike Rodrigo A. Reis
       Electrical Engineering Stundent at University Federal of Pará
       maikerdralcantara@gmail.com
    Created on:
       11 Mar 2020
*/

#include "soft_usart.h"

                                                                                /*
Time to send one bit in us
min 98 us 
max 110 us                                                                      */
#define BIT_DELAY (1000000 / BAUD)

void set_rx()
{
    //PIN 2 is PD2/RXD
    //PIN 3 is PD3/TXD
    clr_bit(DDRD, DD2);  //PD0 is input
    set_bit(PORTD, PD2); //Enable pull-up on PD0
}

void set_tx()
{
    set_bit(PORTD, PD3); //Turn PD1 HIGH
    set_bit(DDRD, DD3);  //PD1 is output
}

void send_char(const uint8_t byte)
{
    PORTD &= ~(1 << PORTD3);
    _delay_us(BIT_DELAY);
    for (bit_mask = 0x01; bit_mask; bit_mask <<= 1)
    {
        if (byte & bit_mask)
        {
            PORTD |= (1 << PORTD3);
        }
        else
        {
            PORTD &= ~(1 << PORTD3);
        }
        _delay_us(BIT_DELAY);
    }
    PORTD |= (1 << PORTD3);
    _delay_us(BIT_DELAY);
}