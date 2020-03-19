/*
    twi.c
    Author:
       Maike Rodrigo A. Reis
       Electrical Engineering Stundent at University Federal of Pará
       maikerdralcantara@gmail.com
    Created on:
       19 Mar 2020
*/

#include "twi.h"

#define SCL_F 100000UL
#define _next_step() TWCR |= (1 << TWINT);
#define _clr_start_bit() TWCR &= ~(1 << TWSTA);
#define _send_stop_cond() TWCR |= (1 << TWSTO);
#define _send_start_cond() TWCR |= (1 << TWSTA);

#define NOT_BUSY 1
#define BUSY 0

#define MAX_ATTEMPTS 20
#define _I2C_ADDR1 32
#define _I2C_ADDR2 33
#define _I2C_ADDR3 34
#define INVALID_RETURN -12

unsigned char new_data;
uint8_t new_addr;
volatile uint8_t bus_busy = NOT_BUSY;
volatile uint8_t iter = 0;
volatile uint8_t step = 0;

bool twi_status()
{
    return bus_busy;
}
void twi_enable_pull_ups(void)
{

    DDRC &= ~((1 << 4) | (1 << 5));
    PORTC = (1 << 4) | (1 << 5);
}
void twi_disable_pull_ups(void)
{

    DDRC &= ~((1 << 4) | (1 << 5));
    PORTC &= ~(1 << 4) | (1 << 5);
}
void twi_enable(void)
{

    sei();
    TWBR = ((F_CPU / SCL_F) - 16) / 2;
    TWSR |= 0x00;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
}
void twi_disable(void)
{

    TWCR &= ~((1 << TWINT) | (1 << TWEN) | (1 << TWIE));
}

_Bool twi_write(unsigned char addr, unsigned char data)
{

    iter = 0;
    new_data = data;
    new_addr = (addr << 1);
    _send_start_cond();
    step = 1;

    do
    {

        _delay_us(25);
    } while ((bus_busy == BUSY) || (step < 2));

    if (step == 4)
    {
        return true;
    }
    return false;
}
unsigned char twi_read(unsigned char addr)
{

    if (bus_busy != NOT_BUSY)
        return INVALID_RETURN;

    iter = 0;
    new_data = 0x00;
    new_addr = (addr << 1) + 1;
    _send_start_cond();
    step = 1;

    do
    {
        _delay_us(25);
    } while ((bus_busy == BUSY) || (step < 2));

    if (step == 4)
    {

        return new_data;
    }
    else
    {

        return INVALID_RETURN;
    }
}

ISR(TWI_vect)
{

    switch (TWSR & 0xF8)
    {
    case TW_START:

        bus_busy = BUSY;
        iter++;
        step = 2;
        TWDR = new_addr;
        _clr_start_bit();
        break;

    case TW_MT_SLA_ACK:

        iter++;
        step = 3;
        TWDR = new_data;
        break;

    case TW_MT_SLA_NACK:

        if (iter < MAX_ATTEMPTS)
        {
            iter++;
            _send_start_cond();
        }
        else
        {
            _send_stop_cond();
            bus_busy = NOT_BUSY;
        }
        break;

    case TW_MT_DATA_ACK:

        iter++;
        step = 4;
        _send_stop_cond();
        bus_busy = NOT_BUSY;
        break;

    case TW_MT_DATA_NACK:

        iter++;
        TWDR = new_data;
        step = 3;
        break;

    case TW_MR_SLA_ACK:

        iter++;
        step = 3;
        TWCR &= ~(1 << TWEA);
        break;

    case TW_MR_DATA_NACK:

        iter++;
        new_data = TWDR;
        step = 4;
        _send_stop_cond();
        _delay_ms(100);
        bus_busy = NOT_BUSY;
        break;

    case TW_MR_SLA_NACK:

        if (iter < MAX_ATTEMPTS)
        {
            iter++;
            _send_start_cond();
        }
        else
        {
            _send_stop_cond();
            bus_busy = NOT_BUSY;
        }
        break;

    case TW_MR_DATA_ACK:

        if (iter < MAX_ATTEMPTS)
        {
            iter++;
            _send_start_cond();
        }
        else
        {
            _send_start_cond();
            bus_busy = NOT_BUSY;
        }
        break;

    case TW_BUS_ERROR:

        iter++;
        _send_start_cond();
        break;

    case TW_REP_START:

        iter++;
        _clr_start_bit();
        break;
    }
    _next_step();
}
