#ifndef __USART_H__
#define __USART_H__

#include "definitions.h"

#ifndef BAUD
#define BAUD 9600
#endif

#ifndef F_CPU 16000000UL
#define F_CPU 16000000UL
#endif

//Assyncronous mode
//UBRR0 = (F_CPU/(16*BAUD)) - 1
//Assyncronous duplex velocity
//UBRR0 = (F_CPU/(8*BAUD)) -1)
//Syncrono Master Mode
//UBRR0 = (F_CPU/(2*BAUD)) -1)

#ifndef MYUBRR F_CPU/16/BAUD - 1
#define MYUBRR F_CPU/16/BAUD - 1
#endif

#define vector_size 5
#define conv_ascii 48

void usart_enable(uint16_t ubrr);
void usart_send(unsigned char dado);
unsigned char usart_receive();
void usart_write(char *c);
void usart_write_flash(const char *c);
void usart_flush(void);

#endif
