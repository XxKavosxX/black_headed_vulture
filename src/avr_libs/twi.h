#ifndef __TWI_H__
#define __TWI_H__
#endif

#ifndef THECODES_INCLUDES__TWI_H_
#define THECODES_INCLUDES__TWI_H_

#include "definitions.h"

ISR(TWI_vect);

bool twi_status();
void twi_enable(void);
void twi_disable(void);
void twi_enable_pull_ups();
void twi_disable_pull_ups(void);
unsigned char twi_read(unsigned char addr);
bool twi_write(unsigned char addr,unsigned char data);

#endif