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
    see doc: https://onlinedocs.microchip.com/pr/GUID-ED37252C-1496-4275-BAEF-5152050ED2C2-en-US-2/index.html  

*/

#include "soft_usart.h"

#define BIT_DELAY (1000000 / BAUD)

volatile _Bool rx_listening = 0;

#define MAX_BUFFER_SIZE 64

uint8_t rx_buffer[MAX_BUFFER_SIZE];
uint8_t rx_buffer2;
volatile uint8_t heap = 0;

uint8_t RX;
uint8_t TX;

void _populate_buffer(uint8_t byte);


char softuart_get_heap()
{
    return (heap + 48);
}

//Only available on PIND 2~7
_Bool softuart_config(uint8_t rx_pin, uint8_t tx_pin)
{
    if (rx_pin >= 2 && rx_pin <= 7)
    {
        //RX
        clr_bit(DDRD, rx_pin);
        set_bit(PORTD, rx_pin);
        RX = rx_pin;

        if ((tx_pin >= 2 && tx_pin <= 7) && (tx_pin != rx_pin))
        {
            //TX
            set_bit(PORTD, tx_pin);
            set_bit(DDRD, tx_pin);
            TX = tx_pin;
        }
        return 1;
    }
    return 0;
}

void softuart_enable_recv()
{
    sei();
    set_bit(PCICR, PCIE2); //Enable int on PIND's
    set_bit(PCMSK2, RX);
}

void softuart_send_char(const uint8_t byte)
{
    clr_bit(PORTD, TX); //send start bit
    _delay_us(BIT_DELAY);

    //Write each bit of byte
    uint8_t b = 8;
    do
    {
        if (byte & _BV(8 - b))
            PORTD |= (1<<TX);
        else
            PORTD &= ~(1<<TX);
        _delay_us(BIT_DELAY);

        b--;
    } while (b > 0);

    set_bit(PORTD, TX); //send stop bit
    _delay_us(BIT_DELAY);
}

void softuart_write(uint8_t *stream)
{
    uint8_t size = strlen(stream);
    uint8_t byte = size;

    do
    {
        softuart_send_char(stream[size - byte]);
        byte--;
    } while (byte > 0);
}

// void softuart_recv(void)
// {
//     clr_bit(PCMSK2, RX);
//     rx_listening = 1;
//     char byte = 0;

//     _delay_us(BIT_DELAY); //start bit delay

//     //Read each bit
//     uint8_t b = 8;
//     do
//     {
//         if (PIND & _BV(RX))
//             byte |= (1 << (8 - b));

//         _delay_us(BIT_DELAY);

//         b--;
//     } while (b > 0);
//     /*
//     This delay may be unnecessary. 
//     According with AVR304 Application Note */
//     //_delay_us(100); //stop bit delay  
//     //_populate_buffer(byte);
//     rx_buffer2 = byte;
    
//     set_bit(PCMSK2, RX);  //enable interrupt on RX
//     rx_listening = 0; //it's ok to read buffer now;
// }

// char softuart_read()
// {
//     while (rx_listening)
//         ;
//     return rx_buffer2;
// }

// void _populate_buffer(uint8_t byte)
// {
//     if (heap == 0)
//         memset(rx_buffer, 0, sizeof(rx_buffer)); //On buffer overflows or new data stream
//                                                  //clean buffer and write first byte of received data
//     rx_buffer[heap] = byte;
//     heap = ++heap % MAX_BUFFER_SIZE;
// }

// ISR(PCINT2_vect)
// {
//     if (!(PIND & _BV(RX))) //If low, falling edge: start bit
//         softuart_recv();
// }
