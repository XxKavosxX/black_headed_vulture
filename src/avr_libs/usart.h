#ifndef __USART_H__
#define __USART_H__

#include "definitions.h"

                                                                                /* 
Assyncronous mode
    UBRR0 = (F_CPU/(16*BAUD)) - 1
Assyncronous duplex velocity
    UBRR0 = (F_CPU/(8*BAUD)) -1)
Syncrono Master Mode
    UBRR0 = (F_CPU/(2*BAUD)) -1)                                                    */
#ifndef MYUBRR
#define MYUBRR F_CPU / 16 / BAUD - 1
#endif

void usart_enable(uint16_t ubrr);
void usart_send(unsigned char dado);
void usart_write(char *c);
void usart_write_flash(const char *c);
void usart_flush(void);

unsigned char *usart_recv();

char *float2char(char *buffer, double d, int precision);
char *int2char(int n);
char *uint2char(uint32_t n);
char *long2char(int32_t n);


#endif
