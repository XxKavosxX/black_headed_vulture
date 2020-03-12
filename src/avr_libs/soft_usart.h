#ifndef __SOFT_USART_H__
#define __SOFT_USART_H__
#include "definitions.h"

#define _pulse_clock_rising() { _delay_us(1); _clr_bit(PORTD, CLOCK_PIN); _delay_us(1); }
#define _pulse_clock_falling() { _delay_us(1); _set_bit(PORTD, CLOCK_PIN); _delay_us(1); }

void set_rx();
void set_tx();
void send_char(const uint8_t byte);

#endif

