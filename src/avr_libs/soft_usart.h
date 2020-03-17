#ifndef __SOFT_USART_H__
#define __SOFT_USART_H__
#include "definitions.h"

bool softuart_config(uint8_t rx_pin, uint8_t tx_pin);
void softuart_enable_recv();
char softuart_get_heap();
void softuart_send_char(const uint8_t byte);
void softuart_write(uint8_t *stream);
void softuart_recv(void);
char *softuart_read();





#endif

