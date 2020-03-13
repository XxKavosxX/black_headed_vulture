/*
    soft_usart.c
    Author:
       Maike Rodrigo A. Reis
       Electrical Engineering Stundent at University Federal of Pará
       maikerdralcantara@gmail.com
    Created on:
       11 Mar 2020
*/

/*
    Mainly at high baud rates, how much time every instruction takes is very important. 
    Bad timing causes failure in receiving data stream. Take care to calculate how many 
    instructios are there between the interrupt EFFECTIVE REGISTER and the code being
    ready to read the pin.
    see doc: https://onlinedocs.microchip.com/pr/GUID-ED37252C-1496-4275-BAEF-5152050ED2C2-en-US-2/index.html  */

#include "soft_usart.h"

/*
Time to send one bit in us
9200 bps
19200 bps start, mid, stop: BIT_DELAY, BIT_DELAY, 20             */
#define BIT_DELAY (1000000 / BAUD)

volatile bool rx_listening = false;

#define MAX_BUFFER_SIZE 150
char rx_buffer[MAX_BUFFER_SIZE];
uint8_t n = 0;

char softuart_get_heap()
{
    return (char)(n + 48);
}

char *softuart_get_buffer()
{
    while (rx_listening)
        ;
    return rx_buffer;
}

void softuart_config(uint8_t rx_pin, uint8_t tx_pin)
{
    //RX
    clr_bit(DDRD, DD2);
    set_bit(PORTD, PD2);
    //TX
    set_bit(PORTD, PD3);
    set_bit(DDRD, DD3);
}

void softuart_enable_recv()
{
    sei();
    set_bit(PCICR, PCIE2);
    set_bit(PCMSK2, PCINT18);
}

void softuart_send_char(const uint8_t byte)
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
uint8_t softuart_recv(void)
{
    clr_bit(PCMSK2, PCINT18);
    rx_listening = true;
    char byte = 0;

    _delay_us(BIT_DELAY); //start bit delay

    //Read each bit
    for (uint8_t b = 0; b <= 7; b++)
    {
        if (PIND & _BV(PIND2))
            byte |= (1 << b);

        _delay_us(BIT_DELAY);
    }
    /*
    At 19200 bps start, mid, stop: BIT_DELAY, BIT_DELAY, 20 works fine.
    Theese magical number (20) achieved by trial and error                  */
    _delay_us(20); //stop bit delay

    _populate_buffer(byte);

    set_bit(PCMSK2, PCINT18); //enable interrupt on RX

    rx_listening = false; //it's ok to read buffer now;

    return byte;
}

void _populate_buffer(uint8_t byte)
{
    if (n == 0)
    {
        //On buffer overflows or new data stream
        //clean buffer and write first byte of received data
        memset(rx_buffer, 0, sizeof(rx_buffer));
        rx_buffer[n] = byte;

        n = ++n % MAX_BUFFER_SIZE;
    }
    else
    {
        rx_buffer[n] = byte;
        n = ++n % MAX_BUFFER_SIZE;
    }
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
        softuart_recv();
    }
}