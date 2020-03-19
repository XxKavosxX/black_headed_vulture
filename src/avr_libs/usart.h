#ifndef __USART_H__
#define __USART_H__

#include "definitions.h"

void usart_enable(uint16_t ubrr);
void usart_send(unsigned char dado);
unsigned char usart_receive();
void usart_write(char *c);
void usart_write_flash(const char *c);
void usart_flush(void);

#endif
