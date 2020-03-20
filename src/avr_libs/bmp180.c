/*
    bmp180.c
    Author:
       Maike Rodrigo A. Reis
       Electrical Engineering Stundent at University Federal of Pará
       maikerdralcantara@gmail.com
    Created on:
       20 Mar 2020
*/

#include "bmp180.h"

int16_t read16(uint8_t addr);

bool bmp180_start(uint8_t oversampling_setting)
{
    if (oversampling_setting > 3)
        oversampling_setting = 3;

    oss = oversampling_setting;

    if (twi_read(0xD0) != 0x55)
        return false;

    ac1 = read16(AC1_ADDR);
    ac2 = read16(AC2_ADDR);
    ac3 = read16(AC3_ADDR);
    ac4 = read16(AC4_ADDR);
    ac5 = read16(AC5_ADDR);
    ac6 = read16(AC6_ADDR);
    b1 = read16(B1_ADDR);
    b2 = read16(B2_ADDR);
    mb = read16(MB_ADDR);
    mc = read16(MC_ADDR);
    md = read16(MD_ADDR);

    return true;
}

int16_t read16(uint8_t addr)
{
    int16_t data;
    data = twi_read(addr);
    data <<= 8;
    data |= twi_read(addr + 1);
    return data;
}

uint16_t bmp180_readUT()
{
    twi_write(0xF4, 0x2E);
    _delay_ms(4.5);
    uint16_t UT;
    UT = read16(0xF6);
}

uint16_t bmp180_readUP()
{

    uint32_t UP;

    twi_write(0xF4, 0x34 << (oss << 6));

    if (oss == 0)
        _delay_ms(4.5);
    else if (oss == 1)
        _delay_ms(7.5);
    else if (oss == 2)
        _delay_ms(13.5);
    else
        _delay_ms(24.5);

    UP = read16(0xF6);

    UP <<= 8;
    UP |= twi_read(0xF6 + 2);
    UP >>= (8 - oss);

    return UP;
}

long _calc_B5(int32_t UT)
{
    int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;
    int32_t X2 = ((int32_t)mc << 11) / (X1 + (int32_t)md);
    return X1 + X2;
}

long bmp180_read_pressure()
{

    int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
    uint32_t B4, B7;

    UP = bmp180_readUP();
    UT = bmp180_readUT();

    B5 = _calc_B5(UT);

    B6 = B5 - 4000;
    X1 = ((int32_t)b2 * ((B6 * B6) >> 12)) >> 11;
    X2 = ((int32_t)ac2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((int32_t)ac1 * 4 + X3) << oss) + 2) / 4;

    X1 = ((int32_t)ac3 * B6) >> 13;
    X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
    B7 = ((uint32_t)UP - B3) * (uint32_t)(50000UL >> oss);

    if (B7 < 0x80000000)
        p = (B7 * 2) / B4;
    else
        p = (B7 / B4) * 2;

    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;

    p = p + ((X1 + X2 + (int32_t)3791) >> 4);
    return p;
}

int32_t bmp180_read_sea_lvl_pressure(float altitude_meters)
{
    float pressure = bmp180_read_pressure();
    return (int32_t)(pressure / pow(1.0 - altitude_meters / 44330, 5.255));
}

float bmp180_read_temperature(void)
{
    int32_t UT, B5;
    float temp;

    UT = bmp180_readUT();

    B5 = _calc_B5(UT);
    temp = (B5 + 8) >> 4;
    temp /= 10;

    return temp;
}

float bmp180_read_altitude(float sea_level_pressure)
{
    float altitude;

    float pressure = bmp180_read_pressure();

    altitude = 44330 * (1.0 - pow(pressure / sea_level_pressure, 0.1903));

    return altitude;
}