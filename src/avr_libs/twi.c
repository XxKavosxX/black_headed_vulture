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
#include "usart.h"

#ifndef SCL_F
#define SCL_F 1600000UL
#endif

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

volatile unsigned char new_data[2]={0x00};
uint8_t new_addr;
volatile uint8_t bus_stt = NOT_BUSY;
volatile uint8_t iter = 0;
volatile uint8_t step = 0;

uint8_t data_cntr = 0;
uint8_t data_size = 0;

void twi_begin()
{
    twi_enable_pull_ups();
    twi_enable();
}

_Bool twi_status()
{
    return bus_stt;
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

#if TWI_DEBUG == 1
    usart_write("\n    | \n    '-> INTERFACE method 'twi_write' \n");
#endif

    iter = 0;
    data_cntr = 0;
    data_size = 1;
    new_data[0] = data;
    new_addr = (addr << 1);

    _send_start_cond();
    step = 1;

    do
    {
        _delay_us(25);
    } while ((bus_stt == BUSY) || (step < 2));

    if (step == 4)
        return 1;
    return 0;
}
_Bool twi_write_bytes(unsigned char addr, unsigned char *data)
{

#if TWI_DEBUG == 1
    usart_write("\n    | \n    '-> INTERFACE method 'twi_write_bytes' \n");
#endif

    iter = 0;
    data_cntr = 0;
    data_size = sizeof(data);
    memcpy(new_data, data, data_size);
    new_addr = (addr << 1);

    _send_start_cond();
    step = 1;

    do
    {
        _delay_us(25);
    } while ((bus_stt == BUSY) || (step < 2));

    if (step == 4)
        return 1;
    return 0;
}

unsigned char twi_read(unsigned char addr)
{

#if TWI_DEBUG == 1
    usart_write("\n    | \n    '-> INTERFACE method 'twi_read'  \n");
#endif

    if (bus_stt == BUSY)
        return NULL;

    iter = 0;
    data_cntr = 0;
    data_size = 1;
    new_data[0] = 0x00;
    new_addr = (addr << 1) + 1;

    _send_start_cond();
    step = 1;

    do
    {
        _delay_us(25);
    } while ((bus_stt == BUSY) || (step < 2)); //Wait for complete reading

    if (step == 4)
    {

// #if TWI_DEBUG == 1
//         usart_write("\n\n         |\n         '-> New data returned: ");
//         usart_write(int2char(new_data[0]));
// #endif
        return new_data[0];
    }
    else
    {

#if TWI_DEBUG == 1
        usart_write("\n         |\n         '-> NULL returned");
#endif

        return NULL;
    }
}

unsigned char *twi_read_bytes(unsigned char addr, uint8_t num)
{

#if TWI_DEBUG == 1
    usart_write("\n    | \n    '-> INTERFACE method 'twi_read_bytes'  \n");
#endif

    if (bus_stt == BUSY)
        return NULL;

    iter = 0;
    data_cntr = 0;
    data_size = num;
    memset(new_data, 0, 2);
    new_addr = (addr << 1) + 1;

    _send_start_cond();
    step = 1;

    do
    {
        _delay_us(50);
    } while ((bus_stt == BUSY) || (step < 2)); //Wait for complete reading

    if (step == 4)
    {
        int16_t aux;
        aux = new_data[0];
        aux <<= 8;
        aux |= new_data[1];

// #if TWI_DEBUG == 1
//         usart_write("\n\n         |\n         '-> New data returned: ");
//         usart_write(int2char(aux));
// #endif

        return new_data;
    }
    else
    {

#if TWI_DEBUG == 1
        usart_write("\n         |\n         '-> NULL returned");
#endif

        return NULL;
    }
}

/*===========================================================================*/

ISR(TWI_vect)
{
    switch (TWSR & 0xF8)
    {
    case TW_START:

#if (TWI_DEBUG == 1)
        usart_write("\n         |\n         '-> TW_START");
#endif

        bus_stt = BUSY;
        iter++;
        step = 2;
        TWDR = new_addr;
        _clr_start_bit();
        break;

    case TW_MT_SLA_ACK:

#if (TWI_DEBUG == 1)
        usart_write(", MT_SLA_ACK");
#endif

        iter++;
        step = 3;
        TWDR = new_data[data_cntr]; //send data0
        data_cntr++;
        break;

    case TW_MT_SLA_NACK:

#if (TWI_DEBUG == 1)
        usart_write(", MT_SLA_NACK");
#endif
        //If SLA+W is sended and NACK is returned
        //try restart 'iter' times.
        if (iter < MAX_ATTEMPTS)
        {
            iter++;
            _send_start_cond();
        }
        else
        {
            _send_stop_cond();
            bus_stt = NOT_BUSY;
        }
        break;

    case TW_MT_DATA_ACK:

#if (TWI_DEBUG == 1)
        usart_write(", MT_DATA_ACK");
#endif
        //If has data to send, send!
        //Otherwise, stopbit
        iter++;
        if (data_cntr < data_size)
        {
            step = 3;
            TWDR = new_data[data_cntr]; //send data1,data2,...,datan
            data_cntr++;
        }
        else
        {
            step = 4;
            _send_stop_cond();
            bus_stt = NOT_BUSY;
        }

        break;

    case TW_MT_DATA_NACK:

#if (TWI_DEBUG == 1)
        usart_write(", MT_DATA_NACK");
#endif

        iter++;
        TWDR = new_data[data_cntr];
        step = 3;
        break;

    case TW_MR_SLA_ACK:

#if (TWI_DEBUG == 1)
        usart_write(", MR_SLA_ACK");
#endif
        iter++;
        step = 3;
        if (data_size > 1)
        {
            TWCR |= (1 << TWEA); //send ACK after receive next data
        }
        else
        {
            TWCR &= ~(1 << TWEA); // send  NACK after receive next data
        }
        break;

    case TW_MR_SLA_NACK:

#if (TWI_DEBUG == 1)
        usart_write(", MR_SLA_NACK");
#endif

        if (iter < MAX_ATTEMPTS)
        {
            iter++;
            _send_start_cond();
        }
        else
        {
            _send_stop_cond();
            bus_stt = NOT_BUSY;
        }
        break;

    case TW_MR_DATA_ACK:

#if (TWI_DEBUG == 1)
        usart_write(", MR_DATA_ACK");
#endif

        iter++;

        if (data_cntr < data_size)
        {

#if TWI_DEBUG == 1
            usart_write(" [TWDR value]:");
            usart_send(TWDR + 48);
#endif

            new_data[data_cntr] = TWDR;
            data_cntr++;
        }

        if (data_cntr == data_size)
        {
            TWCR &= ~(1 << TWEA);
            step = 4;
            _delay_ms(100);
            bus_stt = NOT_BUSY;
            _send_stop_cond();
        }
        else
        {
            TWCR |= (1 << TWEA);
            step = 3;
        }
#if (TWI_DEBUG == 1)
        usart_write("\n");
#endif
        break;

    case TW_MR_DATA_NACK:

#if (TWI_DEBUG == 1)
        usart_write(", MR_DATA_NACK");
#endif

        iter++;
        step = 4;
        new_data[data_cntr] = TWDR; //store last byte received

#if TWI_DEBUG == 1
        usart_write(" [TWDR value]: ");
        usart_send(TWDR + 48);
#endif
        _send_stop_cond();
        _delay_ms(100);
        bus_stt = NOT_BUSY;
        break;

    case TW_BUS_ERROR:

#if (TWI_DEBUG == 1)
        usart_write(", BUS_ERROR");
#endif

        iter++;
        _send_start_cond();

        break;

    case TW_REP_START:

#if (TWI_DEBUG == 1)
        usart_write(", REP_START");
#endif
        iter++;
        _clr_start_bit();

        break;
    }
    _next_step();
}
