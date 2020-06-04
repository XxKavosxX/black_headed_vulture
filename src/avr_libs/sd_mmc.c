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

#define SD_MMC_DEBUG 0

// SD commands
#define CMD0 0
#define CMD0_ARG 0x00000000
#define CMD0_CRC 0x94
#define CMD8 8
#define CMD8_ARG 0x0000001AA
#define CMD8_CRC 0x86
#define CMD9 9
#define CMD9_ARG 0x00000000
#define CMD9_CRC 0x00
#define CMD10 9
#define CMD10_ARG 0x00000000
#define CMD10_CRC 0x00
#define CMD13 13
#define CMD13_ARG 0x00000000
#define CMD13_CRC 0x00
#define CMD17 17
#define CMD17_CRC 0x00
#define CMD24 24
#define CMD24_CRC 0x00
#define CMD55 55
#define CMD55_ARG 0x00000000
#define CMD55_CRC 0x00
#define CMD58 58
#define CMD58_ARG 0x00000000
#define CMD58_CRC 0x00
#define ACMD41 41
#define ACMD41_ARG 0x40000000
#define ACMD41_CRC 0x00

// R1 response bits
#define PARAM_ERROR(X) X & 0b01000000
#define ADDR_ERROR(X) X & 0b00100000
#define ERASE_SEQ_ERROR(X) X & 0b00010000
#define CRC_ERROR(X) X & 0b00001000
#define ILLEGAL_CMD(X) X & 0b00000100
#define ERASE_RESET(X) X & 0b00000010
#define IN_IDLE(X) X & 0b00000001

#define POWER_UP_STATUS(X) X & 0x40
#define CCS_VAL(X) X & 0x40
#define VDD_2728(X) X & 0b10000000
#define VDD_2829(X) X & 0b00000001
#define VDD_2930(X) X & 0b00000010
#define VDD_3031(X) X & 0b00000100
#define VDD_3132(X) X & 0b00001000
#define VDD_3233(X) X & 0b00010000
#define VDD_3334(X) X & 0b00100000
#define VDD_3435(X) X & 0b01000000
#define VDD_3536(X) X & 0b10000000

#define CMD_VER(X) ((X >> 4) & 0xF0)
#define VOL_ACC(X) (X & 0x1F)

#define VOLTAGE_ACC_27_33 0b00000001
#define VOLTAGE_ACC_LOW 0b00000010
#define VOLTAGE_ACC_RES1 0b00000100
#define VOLTAGE_ACC_RES2 0b00001000

#define SD_IN_IDLE_STATE 0x01
#define SD_READY 0x00

#define R3_BYTES 4
#define R7_BYTES 4

#define CMD0_MAX_ATTEMPTS 255
#define CMD55_MAX_ATTEMPTS 255
#define SD_ERROR 1
#define SD_SUCCESS 0
#define SD_MAX_READ_ATTEMPTS 1563
#define SD_MAX_WRITE_ATTEMPTS 3907

#define SD_READ_START_TOKEN 0xFE
#define SD_INIT_CYCLES 80

#define SD_START_TOKEN 0xFE
#define SD_ERROR_TOKEN 0x00

#define SD_DATA_ACCEPTED 0x05
#define SD_DATA_REJECTED_CRC 0x0B
#define SD_DATA_REJECTED_WRITE 0x0D

#define SD_BLOCK_LEN 512

#define SD_TOKEN_OOR(X) X & 0b00001000
#define SD_TOKEN_CECC(X) X & 0b00000100
#define SD_TOKEN_CC(X) X & 0b00000010
#define SD_TOKEN_ERROR(X) X & 0b00000001

#define cs_enable() clr_bit(PORTB, CS);
#define cs_disable() set_bit(PORTB, CS);

void sd_mmc_power_up()
{
#if SD_MMC_DEBUG == 1
    usart_write("\n    | \n    '-> INTERFACE method 'sd_mmc_power_up' \n");
#endif

    // Make sure card is deselected
    cs_disable();

    // Give SD card time to power up
    _delay_ms(1);

    // Send 80 clock cycles to synchronize
    for (uint8_t i = 0; i < 10; i++)
        spi(0xFF);

    // Deselect SD card
    cs_disable();
    spi(0xFF);
}

uint8_t sd_mmc_init()
{
    uint8_t res[5], attempts = 0;

    // Power Up card
    sd_mmc_power_up();

    while ((res[0] = sd_mmc_go_idle()) != SD_IN_IDLE_STATE)
    {
        attempts++;

        if (attempts == CMD0_MAX_ATTEMPTS)
            return SD_ERROR;
    }

    _delay_ms(1);

    sd_mmc_send_if_cond(res);

    if (res[0] != SD_IN_IDLE_STATE)
    {
        return SD_ERROR;
    }

    if (res[4] != 0xAA)
        return SD_ERROR;

    attempts = 0;

    do
    {
        if (attempts == CMD55_MAX_ATTEMPTS)
            return SD_ERROR;

        res[0] = sd_mmc_send_app();

        if (SD_R1_NO_ERROR(res[0]))
            res[0] = sd_mmc_send_op_cond();

        _delay_ms(1);

        attempts++;
    } while (res[0] != SD_READY);

    _delay_ms(1);

    sd_mmc_read_ocr(res);

    return SD_SUCCESS;
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
uint8_t sd_mmc_command(uint8_t index, uint32_t args, uint8_t CRC)
{
#if SD_MMC_DEBUG == 1
    usart_write("\n    | \n    '-> INTERFACE method 'sd_mmc_command' \n");
#endif

    spi(index | 0x40);

    spi((uint8_t)(args >> 24));
    
    spi((uint8_t)(args >> 16));

    spi((uint8_t)(args >> 8));

    spi((uint8_t)(args));

    spi(CRC | 0x01);
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
uint8_t sd_mmc_response()
{
    uint8_t i = 0, res;

    // keep polling until actual data received
    while ((res = spi(0xFF)) == 0xFF)
    {

        i++;

        // if no data received for 8 bytes, break
        if (i > 8)
            break;
    }
    return res;
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
uint8_t sd_mmc_go_idle()
{
#if SD_MMC_DEBUG == 1
    usart_write("\n    | \n    '-> INTERFACE method 'sd_mmc_go_idle' \n");
#endif

    uint8_t response;

    // assert chip select
    spi(0xFF);
    cs_enable();
    spi(0xFF);

    // send CMD0
    sd_mmc_command(CMD0, 0x00, CMD0_CRC);

    // read response
    response = sd_mmc_response();

    // deassert chip select
    spi(0xFF);
    cs_disable();
    spi(0xFF);

    return response;
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
void sd_mmc_read_res7(uint8_t *res)
{
    // read response 1 in R7
    res[0] = sd_mmc_response();

    // if error reading R1, return
    if (res[0] > 1)
        return;

    // read remaining bytes
    res[1] = spi(0xFF);
    res[2] = spi(0xFF);
    res[3] = spi(0xFF);
    res[4] = spi(0xFF);
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
void sd_mmc_read_res3(uint8_t *res)
{
    // read R1
    res[0] = sd_mmc_response();

    // if error reading R1, return
    if (res[0] > 1)
        return;

    // read remaining bytes
    res[1] = spi(0xFF);
    res[2] = spi(0xFF);
    res[3] = spi(0xFF);
    res[4] = spi(0xFF);
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
void sd_mmc_send_if_cond(uint8_t *res)
{
    // assert chip select
    spi(0xFF);
    cs_enable();
    spi(0xFF);

    // send CMD8
    sd_mmc_command(CMD8, CMD8_ARG, CMD8_CRC);

    // read response
    sd_mmc_read_res7(res);

    // deassert chip select
    spi(0xFF);
    cs_disable();
    spi(0xFF);
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
void sd_mmc_read_ocr(uint8_t *res)
{
    // assert chip select
    spi(0xFF);
    cs_enable();
    spi(0xFF);

    // send CMD58
    sd_mmc_command(CMD58, CMD58_ARG, CMD58_CRC);

    // read response
    sd_mmc_read_res3(res);

    // deassert chip select
    spi(0xFF);
    cs_disable();
    spi(0xFF);
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
uint8_t sd_mmc_send_app()
{
    uint8_t response;
    
    // assert chip select
    spi(0xFF);
    cs_enable();
    spi(0xFF);

    // send CMD0
    sd_mmc_command(CMD55, CMD55_ARG, CMD55_CRC);

    // read response
    response = sd_mmc_response();

    // deassert chip select
    spi(0xFF);
    cs_disable();
    spi(0xFF);

    return response;
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
uint8_t sd_mmc_send_op_cond()
{
    uint8_t response;

    // assert chip select
    spi(0xFF);
    cs_enable();
    spi(0xFF);

    // send CMD0
    sd_mmc_command(ACMD41, ACMD41_ARG, ACMD41_CRC);

    // read response
    response = sd_mmc_response();

    // deassert chip select
    spi(0xFF);
    cs_disable();
    spi(0xFF);

    return response;
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
uint8_t sd_mmc_read_block(uint32_t addr, uint8_t *buf, uint8_t *token)
{
    uint8_t response, read;
    uint16_t attempts;

    // set token to none
    *token = 0xFF;

    // assert chip select
    spi(0xFF);
    cs_enable();
    spi(0xFF);

    // send CMD17
    sd_mmc_command(CMD17, addr, CMD17_CRC);

    // read R1
    response = sd_mmc_response();

    // if response received from card
    if (response != 0xFF)
    {
        // wait for a response token (timeout = 100ms)
        attempts = 0;
        while (++attempts != SD_MAX_READ_ATTEMPTS)
            if ((read = spi(0xFF)) != 0xFF)
                break;

        // if response token is 0xFE
        if (read == 0xFE)
        {
            // read 512 byte block
            for (uint16_t i = 0; i < 512; i++)
                *buf++ = spi(0xFF);

            // read 16-bit CRC
            spi(0xFF);
            spi(0xFF);
        }

        // set token to card response
        *token = read;
    }

    // deassert chip select
    spi(0xFF);
    cs_disable();
    spi(0xFF);

    return response;
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
uint8_t sd_mmc_write_block(uint32_t addr, uint8_t *buf, uint8_t *token)
{
    uint8_t attempts, read, res[5];

    // set token to none
    *token = 0xFF;

    // assert chip select
    spi(0xFF);
    cs_enable();
    spi(0xFF);

    // send CMD24
    sd_mmc_command(CMD24, addr, CMD24_CRC);

    // read response
    res[0] = sd_mmc_response();

    // if no error
    if (res[0] == SD_READY)
    {
        // send start token
        spi(SD_START_TOKEN);

        // write buffer to card
        for (uint16_t i = 0; i < SD_BLOCK_LEN; i++)
            spi(buf[i]);

        // wait for a response (timeout = 250ms)
        attempts = 0;
        while (++attempts != SD_MAX_WRITE_ATTEMPTS)
        {
            if ((read = spi(0xFF)) != 0xFF)
            {
                *token = 0xFF;
                break;
            }
        }

        // if data accepted
        if ((read & 0x1F) == 0x05)
        {
            // set token to data accepted
            *token = 0x05;

            // wait for write to finish (timeout = 250ms)
            attempts = 0;
            while (spi(0xFF) == 0x00)
            {
                if (++attempts == SD_MAX_WRITE_ATTEMPTS)
                {
                    *token = 0x00;
                    break;
                }
            }
        }
    }

    // deassert chip select
    spi(0xFF);
    cs_disable();
    spi(0xFF);

    return res[0];
}

/*=========================================================================|
|                                                                          |
|                                                                          |
|                                                                          |
'=========================================================================*/
void sd_print_r1(uint8_t res)
{
    if (res & 0b10000000)
    {
        usart_write("\tError: MSB = 1\r\n");
        return;
    }
    if (res == 0)
    {
        usart_write("\tCard Ready\r\n");
        return;
    }
    if (PARAM_ERROR(res))
        usart_write("\tParameter Error\r\n");
    if (ADDR_ERROR(res))
        usart_write("\tAddress Error\r\n");
    if (ERASE_SEQ_ERROR(res))
        usart_write("\tErase Sequence Error\r\n");
    if (CRC_ERROR(res))
        usart_write("\tCRC Error\r\n");
    if (ILLEGAL_CMD(res))
        usart_write("\tIllegal Command\r\n");
    if (ERASE_RESET(res))
        usart_write("\tErase Reset Error\r\n");
    if (IN_IDLE(res))
        usart_write("\tIn Idle State\r\n");
}

void sd_print_r7(uint8_t *res)
{
    sd_print_r1(res[0]);

    if (res[0] > 1)
        return;

    usart_write("\tCommand Version: ");
    usart_write(int2hex(CMD_VER(res[1])));
    usart_write("\r\n");

    usart_write("\tVoltage Accepted: ");
    if (VOL_ACC(res[3]) == VOLTAGE_ACC_27_33)
        usart_write("2.7-3.6V\r\n");
    else if (VOL_ACC(res[3]) == VOLTAGE_ACC_LOW)
        usart_write("LOW VOLTAGE\r\n");
    else if (VOL_ACC(res[3]) == VOLTAGE_ACC_RES1)
        usart_write("RESERVED\r\n");
    else if (VOL_ACC(res[3]) == VOLTAGE_ACC_RES2)
        usart_write("RESERVED\r\n");
    else
        usart_write("NOT DEFINED\r\n");

    usart_write("\tEcho: ");
    usart_write(int2hex(res[4]));
    usart_write("\r\n");
}

void sd_print_r3(uint8_t *res)
{
    sd_print_r1(res[0]);

    if (res[0] > 1)
        return;

    usart_write("\tCard Power Up Status: ");
    if (POWER_UP_STATUS(res[1]))
    {
        usart_write("READY\r\n");
        usart_write("\tCCS Status: ");
        if (CCS_VAL(res[1]))
        {
            usart_write("1\r\n");
        }
        else
            usart_write("0\r\n");
    }
    else
    {
        usart_write("BUSY\r\n");
    }

    usart_write("\tVDD Window: ");
    if (VDD_2728(res[3]))
        usart_write("2.7-2.8, ");
    if (VDD_2829(res[2]))
        usart_write("2.8-2.9, ");
    if (VDD_2930(res[2]))
        usart_write("2.9-3.0, ");
    if (VDD_3031(res[2]))
        usart_write("3.0-3.1, ");
    if (VDD_3132(res[2]))
        usart_write("3.1-3.2, ");
    if (VDD_3233(res[2]))
        usart_write("3.2-3.3, ");
    if (VDD_3334(res[2]))
        usart_write("3.3-3.4, ");
    if (VDD_3435(res[2]))
        usart_write("3.4-3.5, ");
    if (VDD_3536(res[2]))
        usart_write("3.5-3.6");
    usart_write("\r\n");
}

void sd_print_data_token(uint8_t token)
{
    if (SD_TOKEN_OOR(token))
        usart_write("\tData out of range\r\n");
    if (SD_TOKEN_CECC(token))
        usart_write("\tCard ECC failed\r\n");
    if (SD_TOKEN_CC(token))
        usart_write("\tCC Error\r\n");
    if (SD_TOKEN_ERROR(token))
        usart_write("\tError\r\n");
}