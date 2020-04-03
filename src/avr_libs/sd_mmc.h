#ifndef __SD_MMC_H__
#define __SD_MMC_H__
#include "spi.h"
void sd_mmc_enable();
static uint8_t sd_mmc_send_command(uint8_t index, uint16_t args, uint16_t args2, uint8_t CRC);
uint8_t sd_mmc_recv_response(uint8_t response);
uint8_t sd_mmc_start();

#endif