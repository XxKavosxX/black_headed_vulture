#include "soft_usart.h"
#include "util/delay.h"
#include "bmp180.h"
#include "usart.h"
int main()
{
    usart_enable(MYUBRR);
    DDRB = 0xFF;
    while (1)
    {

       //char buffer[64];
        //int ret = snprintf(buffer, sizeof buffer, "%f", bmp180_read_pressure());


        usart_write("Pressure");
        //usart_write(buffer);
        
        _delay_ms(1000);
    }
}