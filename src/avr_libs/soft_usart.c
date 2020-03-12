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
#include "usart.h"

/*
Time to send one bit in us
min 98 us 
max 110 us                                                                      */
#define BIT_DELAY (1000000 / BAUD)
volatile bool is_listening = false;

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

void enable_soft_usart_recv()
{
    sei();
    set_bit(PCICR, PCIE2);
    set_bit(PCMSK2, PCINT18);
}

void send_char(const uint8_t byte)
{
    clr_bit(PORTD, PORTD3); //send start bit
    _delay_us(BIT_DELAY);

    //Write each bit of byte
    for (uint8_t b = 0; b < 8; ++b)
    {
        if (byte & _BV(b))
            set_bit(PORTD, PORTD3);
        else
            clr_bit(PORTD, PORTD3);
        _delay_us(BIT_DELAY);
    }

    set_bit(PORTD, PORTD3); //send stop bit
    _delay_us(BIT_DELAY);
}
uint8_t recv_char(void)
{
    while (!is_listening) //do nothing until Start bit
        ;

    uint8_t byte = 0;
    _delay_us(BIT_DELAY); //Wait start bit

    //Read each bit
    for (uint8_t b = 0; b <= 7; ++b)
    {
        if (PIND & _BV(PIND2))
            byte |= (1 << b);

        _delay_us(BIT_DELAY);
    }

    _delay_us(BIT_DELAY);//Wait stop bit

    is_listening = false;
    set_bit(PCMSK2, PCINT18); //enable interrupt

    return byte;
}

ISR(PCINT2_vect)
{
    //Piece of code to debug INTERRUPT
    if (PIND & (1 << PIND2)) //IF HIGH, rising edge: is a STOP BIT
    {
        //do anything
    }
    else //ELSE LOW, falling edge: is a START BIT
    {
        clr_bit(PCMSK2, PCINT18); //disable new interrupt
        is_listening = true;      //Flag of recv_char() function
    }
}