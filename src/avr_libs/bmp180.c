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
#include "usart.h"

#define BMP_DEBUG 0

uint8_t read8(uint8_t addr);

uint16_t read16(uint8_t addr);

void write16(uint8_t addr, uint8_t data);

static uint8_t oss;

int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;

uint16_t ac4, ac5, ac6;

_Bool bmp180_start(uint8_t oversampling_setting)
{
#if BMP_DEBUG == 1
    usart_write("\n FUNCTION 'bmp180_start' \n");
#endif
    if (oversampling_setting > 3)
        oversampling_setting = 3;

    oss = oversampling_setting;

    if (read8(0xD0) != 0x55)
    {

#if BMP_DEBUG == 1
        usart_write("\n |\n '-> 'Not Connected' \n");
#endif
        return 0;
    }
    else
    {

#if BMP_DEBUG == 1
        usart_write("\n |\n '-> 'Connected' \n");
#endif
    }

#if BMP_DEBUG == 1
    usart_write("\n |\n '-> 'Read Data Registers' \n");
#endif

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

#if BMP_DEBUG == 1
    usart_write("\n       ac1:");
    usart_write(int2char(ac1));
    usart_write("\n       ac2:");
    usart_write(int2char(ac2));
    usart_write("\n       ac3:");
    usart_write(int2char(ac3));
    usart_write("\n       ac4:");
    usart_write(uint2char(ac4));
    usart_write("\n       ac5:");
    usart_write(uint2char(ac5));
    usart_write("\n       ac6:");
    usart_write(uint2char(ac6));
    usart_write("\n       b1:");
    usart_write(int2char(b1));
    usart_write("\n       b2:");
    usart_write(int2char(b2));
    usart_write("\n       mb:");
    usart_write(int2char(mb));
    usart_write("\n       mc:");
    usart_write(int2char(mc));
    usart_write("\n       md:");
    usart_write(int2char(md));
    usart_write("\n");
#endif

    return 1;
}

uint8_t read8(uint8_t addr)
{
#if BMP_DEBUG == 1
    usart_write("\n      |\n      '-> FUNCTION 'read8' \n");
#endif

    uint8_t data;
    twi_write(BMP180_I2CADDR, addr);
    data = twi_read(BMP180_I2CADDR);
    return data;
}

void write16(uint8_t addr, uint8_t data)
{
#if BMP_DEBUG == 1
    usart_write("\n      |\n      '-> FUNCTION 'write16' \n");
#endif

    unsigned char buff[2] = {addr, data};
    twi_write_bytes(BMP180_I2CADDR, buff);
}

uint16_t read16(uint8_t addr)
{
#if BMP_DEBUG == 1
    usart_write("\n      |\n      '-> FUNCTION 'read16' \n");
#endif

    uint16_t aux = 0;
    uint8_t *data;

    twi_write(BMP180_I2CADDR, addr);
    data = twi_read_bytes(BMP180_I2CADDR, 2);

    aux = data[0];
    aux <<= 8;
    aux |= data[1];

    return aux;
}

uint32_t bmp180_readUT()
{
#if (BMP_DEBUG == 1)
    usart_write("\n   FUNCTION 'readUT' \n");
#endif

    uint32_t UT;
    write16(0xF4, 0x2E);
    _delay_ms(4.5 + 0.5);
    UT = read16(0xF6);

    return UT;
}

uint32_t bmp180_readUP()
{
#if (BMP_DEBUG == 1)
    usart_write("\n   FUNCTION 'readUP' \n");
#endif

    uint64_t UP;

    write16(0xF4, 0x34 + (oss << 6));

    if (oss == 0)
        _delay_ms(4.5 + 0.5);
    else if (oss == 1 + 0.5)
        _delay_ms(7.5 + 0.5);
    else if (oss == 2 + 0.5)
        _delay_ms(13.5 + 0.5);
    else
        _delay_ms(24.5);

    UP = read16(0xF6);
    UP <<= 8;
    //WARNING
    UP |= read8(0xF6+2);
    UP >>= (8 - oss);

    return UP;
}

int32_t _calc_B5(int32_t UT)
{
    int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;

    int32_t X2 = ((int32_t)mc << 11) / (X1 + (int32_t)md);

    return X1 + X2;
}

float bmp180_read_temperature(void)
{
    int32_t UT, B5;
    float temp;
    int8_t buff[20];

    UT = bmp180_readUT();
    B5 = _calc_B5(UT);
    temp = (B5 + 8) >> 4;
    temp /= 10;

#if BMP_DEBUG == 1
    usart_write("\n    T(°C):");
    usart_write(long2char(temp));
    usart_write("\n");
#endif

    return temp;
}

int32_t bmp180_read_pressure()
{

    int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
    uint32_t B4, B7;

#if BMP_DEBUG == 1
    usart_write("\n FUNCTION 'bmp180_read_pressure' \n");
    usart_write("\n |\n '-> 'Calculate true pressure' \n");
#endif

    UT = bmp180_readUT();
    UP = bmp180_readUP();
    B5 = _calc_B5(UT);
    B6 = B5 - 4000;
    X1 = ((int32_t)b2 * ((B6 * B6) >> 12)) >> 11;
    X2 = ((int32_t)ac2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((ac1 * 4 + X3)) << oss) + 2) / 4;

#if BMP_DEBUG == 1
    usart_write("\n       UT:");
    usart_write(long2char(UT));
    usart_write("\n       UP:");
    usart_write(long2char(UP));
    usart_write("\n       B5:");
    usart_write(long2char(B5));
    usart_write("\n       B6:");
    usart_write(long2char(B6));
    usart_write("\n       X1:");
    usart_write(long2char(X1));
    usart_write("\n       X2:");
    usart_write(long2char(X2));
    usart_write("\n       X3:");
    usart_write(long2char(X3));
    usart_write("\n       B3:");
    usart_write(long2char(B3));
#endif

    X1 = ((int32_t)ac3 * B6) >> 13;
    X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
    B7 = ((uint32_t)UP - B3) * (uint32_t)(50000 >> 3);
    if (B7 < 0x80000000)
        p = (B7 * 2) / B4;
    else
        p = (B7 / B4) * 2;

#if BMP_DEBUG == 1
    usart_write("\n       X1:");
    usart_write(long2char(X1));
    usart_write("\n       X2:");
    usart_write(long2char(X2));
    usart_write("\n       X3:");
    usart_write(long2char(X3));
    usart_write("\n       B4:");
    usart_write(long2char(B4));
    usart_write("\n       B7:");
    usart_write(long2char(B7));
    usart_write("\n       p:");
    usart_write(long2char(p));
#endif

    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    p = p + ((X1 + X2 + (int32_t)3791) >> 4);

#if BMP_DEBUG == 1
    usart_write("\n       X1:");
    usart_write(long2char(X1));
    usart_write("\n       X1:");
    usart_write(long2char(X1));
    usart_write("\n       X2:");
    usart_write(long2char(X2));
    usart_write("\n       p:");
    usart_write(long2char(p));
    usart_write("\n");
#endif

    return p;
}

int32_t bmp180_read_sea_lvl_pressure(float altitude_meters)
{
    float pressure = bmp180_read_pressure();
    return (int32_t)(pressure / pow(1.0 - altitude_meters / 44330, 5.255));
}

float bmp180_read_altitude(float sea_level_pressure)
{
    float altitude;

    float pressure = bmp180_read_pressure();

    altitude = 44330 * (1.0 - pow(pressure / sea_level_pressure, 0.1903));

    return altitude;
}