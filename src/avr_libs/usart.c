/*
	usart.c
	Author:
	   Maike Rodrigo A. Reis
	   Electrical Engineering Stundent at University Federal of Pará
	   maikerdralcantara@gmail.com
	Created on:
	   11 Mar 2020
*/

#include "usart.h"

void usart_enable(uint16_t ubrr)
{

	UBRR0H = (unsigned char)(ubrr >> 8); //take ATmega328P datasheet
	UBRR0L = (unsigned char)ubrr;

	UCSR0A = 0;

	UCSR0A = 0;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void usart_send(unsigned char data)
{

	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = data;
}

unsigned char usart_receive()
{

	while (!(UCSR0A & (1 << RXC0)))
		;
	return UDR0;
}

void usart_write(char *c)
{

	for (; *c != 0; c++)
		usart_send(*c);
}

void usart_write_flash(const char *c)
{

	for (; pgm_read_byte(&(*c)) != 0; c++)
		usart_send(pgm_read_byte(&(*c)));
}

void usar_flush(void)
{
	unsigned char dummy;
	while (UCSR0A & (1 << RXC0))
		dummy = UDR0;
}
