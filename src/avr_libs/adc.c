/*
    adc.c
    Author:
       Maike Rodrigo A. Reis
       Electrical Engineering Stundent at University Federal of Pará
       maikerdralcantara@gmail.com
    Created on:
       10 Mar 2020
*/

#include "adc.h"

uint8_t state = 0;

void adc_enable(uint8_t mode, uint8_t channel, uint8_t prescaler)
{
    if (adc_mode(mode) & adc_select_channel(channel) & adc_select_clk_freq(prescaler))
        state = 1;
}

_Bool adc_get_status()
{
    return state;
}

/* 
 *  MODES
 *  0 0  AREF, internal reference voltage off
 *  0 1  AVCC, need to employ a capacitor between GND and AREFF
 *  1 0  RESERVED
 *  1 1  1.1v internal voltage as reference. Need to employ a capacitor between GND and AREFF 
*/
_Bool adc_mode(uint8_t mode)
{

    ADMUX = 0x00;

    if (!(mode >= 0 && mode <= 3 && mode != 2))
        return false;

    ADMUX |= (mode << 6) & AD_REF_MASK;

    return true;
}

_Bool adc_select_channel(uint8_t channel)
{
    if (!(channel >= 0 && channel <= 6))
        return false;

    ADMUX &= (0xF0 | channel);

    return true;
}

_Bool adc_select_clk_freq(uint8_t prescaler)
{
    if (!(prescaler >= 0 && prescaler <= 7))
        return false;

    ADCSRA = 0x00;

    ADCSRA = (1 << ADEN) | (prescaler);

    return true;
}

char adc_read()
{
    if (!adc_get_status())
        return '0';

    ADCSRA |= (1 << ADSC);

    while (tst_bit(ADCSRA, ADSC))
        ;

    return (ADCH);
}

char adc_read_internal_temp()
{
    ADCSRA = 0x00;

    ADCSRA = (1 << ADEN) | (0x08);
    
    adc_read();
}
