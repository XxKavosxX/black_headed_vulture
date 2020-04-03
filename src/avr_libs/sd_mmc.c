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
    cs_disable();
    spi_enable();
}
static uint8_t sd_mmc_send_command(uint8_t index, uint16_t args, uint16_t args2, uint8_t CRC)
{
#if SD_MMC_DEBUG == 1
    usart_write("\n    | \n    '-> INTERFACE method 'sd_mmc_send_command' \n");
#endif
    uint8_t response;
    spi_send_recv(index);
    spi_send_recv((args >> 8) && 0xFF);
    spi_send_recv((args) && 0xFF);
    spi_send_recv((args2 >> 8) && 0xFF);
    spi_send_recv((args2) && 0xFF);
    spi_send_recv(CRC);
    spi_send_recv(0xFF); // Wait response
    response = spi_send_recv(0xFF); // Get response
    return response;
}

uint8_t sd_mmc_start()
{
#if SD_MMC_DEBUG == 1
    usart_write("\n    | \n    '-> INTERFACE method 'sd_mmc_start' \n");
#endif

    set_bit(PORTB, MOSI); //Make sure MISO is HIGH during writing
    cs_disable();         //Drive CS to low

    /* Send 80 pulses of clock */
    uint8_t i = 10;
    do
    {
        spi_send_recv(0xFF);
        i--;
    } while (i > 0);

    cs_enable();
    /* Send GO_IDLE_STATE */
    uint8_t response = sd_mmc_send_command(0x40, 0x00, 0x00, 0x95);
    /* Verify if is in idle state */
    if (response == 0)
    {
#if SD_MMC_DEBUG == 1
        usart_write("\n         |\n         '-> 1 returned");
#endif
        cs_disable();
        return 1;
    }

    cs_disable();
    spi_send_recv(0xFF);
    cs_enable();

    /* Send SEND_OP_COND */
    i = 0xFF;
    uint8_t response2 = 1;
    do
    {
        i--;
        response2 = sd_mmc_send_command(0x41, 0x00, 0x00, 0xFF);
    } while ((response2 != 0) && (i != 0));

    if (i == 1)
    {
#if SD_MMC_DEBUG == 1
        usart_write("\n         |\n         '-> 2 returned");
#endif
        cs_disable();
        return 2;
    }

    cs_disable();
    spi_send_recv(0xFF);

    return 0;
}
