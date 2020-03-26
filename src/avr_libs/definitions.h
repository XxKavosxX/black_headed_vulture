#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif
#define F_CPU 16000000UL // or whatever may be your frequency
#define BAUD 9600
#define __DELAY_BACKWARD_COMPATIBLE__
#define __DELAY_ROUND_DOWN__

#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <stdio.h>


#define set_bit(y, bit)         (y|= (1<<bit))
#define clr_bit(y, bit)         (y&= ~(1<<bit))
#define cpl_bit(y, bit)         (y^= (1<<bit))
#define tst_bit(y, bit)         (y&(1<<bit))
#define get_high_bits(y) ((y&0xF0)>>4)
#define get_low_bits(y) (y&0x0F)

// char *float2char(char *buffer, double d, int precision);
// char* int2char(int number);
// char* long2char(uint32_t number);

#endif