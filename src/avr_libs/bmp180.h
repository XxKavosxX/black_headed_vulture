#ifndef __BMP180_H__
#define __BMP180_H__
#include "definitions.h"
#include "twi.h"

//E²PROM address of calibration data contain 16bits
//Below are MSB address of calibration data MSB.
//LSB=MSB+1
#define AC1_ADDR 0xAA
#define AC2_ADDR 0xAC
#define AC3_ADDR 0xAE
#define AC4_ADDR 0xB0
#define AC5_ADDR 0xB2
#define AC6_ADDR 0xB4
#define B1_ADDR 0xB6
#define B2_ADDR 0xB8
#define MB_ADDR 0xBA
#define MC_ADDR 0xBC
#define MD_ADDR 0xBE

bool bmp180_start(uint8_t oversampling_setting);


long bmp180_read_pressure();
float bmp180_read_temperature(void);
float bmp180_read_altitude(float sea_level_pressure);
int32_t bmp180_read_sea_lvl_pressure(float altitude_meters);

uint8_t oss;
int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
uint16_t ac4, ac5, ac6;

#endif