#ifndef __SOFT_USART_H__
#define __SOFT_USART_H__
#include "definitions.h"

//void _populate_buffer(uint8_t byte);
void softuart_config(uint8_t rx_pin, uint8_t tx_pin);
void softuart_enable_recv();

char *softuart_get_buffer();
char softuart_get_heap();

uint8_t softuart_recv(void);
void softuart_send_char(const uint8_t byte);

#endif

