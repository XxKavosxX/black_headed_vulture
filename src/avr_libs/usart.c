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

uint8_t n = 0;
unsigned char rcvd_char[200];

void usart_enable(uint16_t ubrr)
{
	UBRR0H = (ubrr >> 8); // Take ATmega328P datasheet
	UBRR0L = ubrr;
	UCSR0A = 0;

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0B |= (1 << RXCIE0); //Enable receiver interrupt
	sei();
}

void usart_send(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = data;
}

unsigned char *usart_recv()
{
	while (!(UCSR0A & (1 << RXC0)))
		;
	return rcvd_char;
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

void usart_flush(void)
{
	unsigned char dummy = "";
	while (UCSR0A & (1 << RXC0))
		dummy = UDR0;
}

ISR(USART_RX_vect)
{
	n++;
	rcvd_char[n] = UDR0; // Read data from the RX buffer
	UDR0 = rcvd_char[n]; // Write the data to the TX buffer
}

char *float2char(char *buffer, double d, int precision)
{
	long wholePart = (long)d;
	itoa(wholePart, buffer, 10);
	if (precision > 0)
	{
		char *endOfString = buffer;
		while (*endOfString != '\0')
			endOfString++;
		*endOfString++ = '.';
		if (d < 0)
		{
			d *= -1;
			wholePart *= -1;
		}
		double fraction = d - wholePart;
		while (precision > 0)
		{
			fraction *= 10;
			wholePart = (long)fraction;
			*endOfString++ = '0' + wholePart;
			fraction -= wholePart;
			precision--;
		}
		*endOfString = '\0';
	}
	return buffer;
}
char *int2char(int numero)
{
	static char buffer[10];
	itoa(numero, buffer, 10);
	return buffer;
}
char *long2char(uint32_t numero)
{
	static char buffer[10];
	utoa(numero, buffer, 10);
	return buffer;
}
