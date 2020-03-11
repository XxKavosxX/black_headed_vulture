#ifndef __ADC_H__
#define __ADC_H__

#include "definitions.h"

// AREF, Vref off
#define AREF_MODE 0x00
// AVCC. Put a 100nF capacitor in AREF to GND
#define AVCC_MODE 0x01
// Internal Vcc 1.1V . Put a 100nF capacitor in AREF to GND
#define IREF_MODE 0x03
#define AD_REF_MASK 0xC0

 void adc_enable(uint8_t mode, uint8_t channel, uint8_t prescaler);
_Bool adc_status();
_Bool adc_mode(uint8_t mode);
_Bool adc_select_channel(uint8_t channel);
_Bool adc_select_clk_freq(uint8_t prescaler);
 char adc_read();
 char adc_read_internal_temp();


 // If at 10bits the maximum sample rate is 15kSPS and each sample uses 13 clock cycles 
 // then the maximum clock frequency will be
 //  m_clk = 15k x 13 = 195k
 // The source clock will be divided by x to clock adc
 //  prescaler = F_CPU / m_clk  = 16Mhz / 195kHz = 82
 // The closest value is 128 that will result 9,6 kSPS

#endif
