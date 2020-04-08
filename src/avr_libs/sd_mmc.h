#ifndef __SD_MMC_H__
#define __SD_MMC_H__
#include "spi.h"


#define SD_R1_NO_ERROR(X)   X < 0x02

void sd_mmc_power_up();
uint8_t sd_mmc_init();

static uint8_t sd_mmc_command(uint8_t index, uint32_t args, uint8_t CRC);
uint8_t sd_mmc_response();

uint8_t sd_mmc_go_idle();
void sd_mmc_send_if_cond(uint8_t *res);
void sd_mmc_read_ocr(uint8_t *res);
uint8_t sd_mmc_send_app();
uint8_t sd_mmc_send_op_cond();

uint8_t sd_mmc_read_block(uint32_t addr, uint8_t *buf, uint8_t *token);
uint8_t sd_mmc_write_block(uint32_t addr, uint8_t *buf, uint8_t *token);

void sd_print_r1(uint8_t res);
void sd_print_r7(uint8_t *res);
void sd_print_r3(uint8_t *res);
void sd_print_data_token(uint8_t token);

#endif