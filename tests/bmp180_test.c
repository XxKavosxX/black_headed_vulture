#include "soft_usart.h"
#include "util/delay.h"
#include "bmp180.h"
#include "usart.h"

int main()
{
    bmp_main();
}
bmp_main()
{
    twi_begin();
    usart_enable(MYUBRR);
    DDRB = 0xFF;
    bmp180_start(3);
    char b[10];

    while (1)
    {
        usart_write("\n  Temperature: ");
        usart_write(float2char(b, bmp180_read_temperature(), 2));
        usart_write("°C");

        usart_write("\n  Altitude: ");
        usart_write(float2char(b, bmp180_read_altitude(101325), 2));
        usart_write("m");

        usart_write("\n  Pressure: ");
        usart_write(long2char(bmp180_read_pressure()));
        usart_write("Pa");

        usart_write("\n  Pressure at Sea lvl ( Calculated ): ");
        usart_write(long2char(bmp180_read_sea_lvl_pressure(0)));
        usart_write("Pa");

        _delay_ms(500);
    }
}

bmp_test_start()
{
    twi_begin();
    usart_enable(MYUBRR);
    DDRB = 0xFF;
    bmp180_start(3);
    while (1)
    {
        _delay_ms(500);
    }
}
bmp_test_sea_lvl_pressure()
{
    twi_begin();
    usart_enable(MYUBRR);
    DDRB = 0xFF;
    bmp180_start(3);

    while (1)
    {

        usart_write("\n  Pressure at Sea lvl ( Calculated ): ");
        usart_write(long2char(bmp180_read_sea_lvl_pressure(0)));
        usart_write("Pa");

        _delay_ms(200);
    }
}

bmp_test_read_altitude()
{
    twi_begin();
    usart_enable(MYUBRR);
    DDRB = 0xFF;
    bmp180_start(3);
    char b[10];
    while (1)
    {
        usart_write("\n  Altitude: ");
        usart_write(float2char(b, bmp180_read_altitude(101325), 2));
        usart_write("m");
    }
}
bmp_test_read_temperature()
{
    twi_begin();
    usart_enable(MYUBRR);

    DDRB = 0xFF;
    bmp180_start(3);
    char b[10];
    while (1)
    {
        usart_write("\n  Temperature: ");
        usart_write(float2char(b, bmp180_read_temperature(), 2));
        usart_write("°C");
        _delay_ms(200);
    }
}
bmp_test_read_pressure()
{
    twi_begin();
    usart_enable(MYUBRR);
    DDRB = 0xFF;
    bmp180_start(3);

    while (1)
    {
        usart_write("\n  Pressure: ");
        usart_write(long2char(bmp180_read_pressure()));
        usart_write("Pa");

        _delay_ms(200);
    }
}

bmp_test_readUT()
{
    twi_begin();
    usart_enable(MYUBRR);
    DDRB = 0xFF;
    bmp180_start(0);
    bmp180_readUT();

    while (1)
    {
        _delay_ms(5000);
    }
}
bmp_test_readUP()
{
    twi_begin();
    usart_enable(MYUBRR);
    DDRB = 0xFF;
    bmp180_start(3);
    bmp180_readUP();
    while (1)
    {
        _delay_ms(100);
    }
}