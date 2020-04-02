/*
    sd_mmc.c
    Author:
       Maike Rodrigo A. Reis
       Electrical Engineering Stundent at University Federal of Pará
       maikerdralcantara@gmail.com
    Created on:
       02 Apr 2020
*/
#include "sd_mmc.h"
#include "usart.h"

#define SD_MMC_DEBUG 1

#define cs_enable() clr_bit(PORTB, CS);
#define cs_disable() set_bit(PORTB, CS);

void sd_mmc_enable()
{
    cs_enable();
    spi_enable();
}
void sd_mmc_send_command(uint8_t index, uint16_t args, uint16_t args2, uint8_t CRC)
{
    spi_send_recv((uint8_t)(args >> 8));
    spi_send_recv((uint8_t)(args));
    spi_send_recv((uint8_t)(args2 >> 8));
    spi_send_recv((uint8_t)(args2));
    spi_send_recv(CRC);
}
uint8_t sd_mmc_recv_response(uint8_t response)
{
    uint8_t cont = 80;

    do
    {
        if (spi_send_recv(0xFF) == response)
            return 1;
        cont--;
    } while (cont != 0);
    return 0;
}

uint8_t sd_mmc_start()
{
#if SD_MMC_DEBUG == 1
    usart_write("sd_mmc_start");
#endif

    /* Send 80 pulses of clock */
    set_bit(PORTB, CS);
    //set_bit(PORTB, MISO);

    for (uint8_t i = 0; i < 10; i++)
        spi_send_recv(0xFF);

    clr_bit(PORTB, CS);

    /* Send GO_IDLE_STATE */
    sd_mmc_send_command(0x40, 0x00, 0x00, 0x95);

    uint8_t response = sd_mmc_recv_response(0x01);
    usart_send(response + 48);

    /* Verify if is in idle state */
    if (response==0)
    {
        cs_disable();
        return 1;
    }
    usart_write("pass0");

    cs_disable();
    spi_send_recv(0xFF);
    cs_enable();

    usart_write("pass1");

    /* Send SEND_OP_COND */
    uint8_t i = 0xFF;
    do
    {
        i--;
        sd_mmc_send_command(0x41, 0x00, 0x00, 0xFF);
    } while ((sd_mmc_recv_response(0x00) != 1) && (i != 0));

    if (i == 0)
    {
        cs_disable();
        return 2;
    }
    usart_write("pass1");

    cs_disable();
    spi_send_recv(0xFF);

    return 0;
}