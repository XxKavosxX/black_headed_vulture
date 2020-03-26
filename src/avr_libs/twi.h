#ifndef __TWI_H__
#define __TWI_H__
#endif

#ifndef THECODES_INCLUDES__TWI_H_
#define THECODES_INCLUDES__TWI_H_

#include "definitions.h"

#define TWI_DEBUG 0

ISR(TWI_vect);

void twi_begin();
_Bool twi_status();
void twi_enable(void);
void twi_disable(void);
void twi_enable_pull_ups();
void twi_disable_pull_ups(void);

unsigned char twi_read(unsigned char addr);

unsigned char *twi_read_bytes(unsigned char addr, uint8_t num);

_Bool twi_write(unsigned char addr, unsigned char data);

_Bool twi_write_bytes(unsigned char addr, unsigned char *data);

#endif