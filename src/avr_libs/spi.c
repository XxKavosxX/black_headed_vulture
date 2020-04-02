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

uint8_t data;

void spi_enable()
{
   set_bit(DDRB, CLK);  //Set as output
   clr_bit(DDRB, MISO); //Set as input
   set_bit(DDRB, MOSI); //Set as output
   set_bit(DDRB, CS);   //Set as output
   set_bit(PORTB, CS);  //Set as output

   SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1); //Spi enable, Master Select, Frequency F_CPU/64
}
uint8_t spi_send_recv(uint8_t data)
{
   SPDR = data;                //Start a transmission
   while (!(SPDR & _BV(SPIF))) //Wait for exchange of data
      ;
   return SPDR; //Return received data from Slave
}