/*
    spi.c
    Author:
       Maike Rodrigo A. Reis
       Electrical Engineering Stundent at University Federal of Pará
       maikerdralcantara@gmail.com
    Created on:
       02 Apr 2020
*/

/* SPI as MASTER implementation */

#include "spi.h"
#include "usart.h"

void spi_enable()
{
    DDRB |= (1 << CS) | (1 << MOSI) | (1 << CLK);

    // enable pull up resistor in MISO
    PORTB |= (1 << MISO);

    // enable SPI, set as master, and clock to fosc/128
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}
uint8_t spi(uint8_t data)
{
   SPDR = data; //Start a transmission

   while(!(SPSR & (1 << SPIF))); //Wait for exchange of data
   
   return SPDR; //Return received data from Slave
}