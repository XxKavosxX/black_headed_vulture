#include "soft_usart.h"
#include "util/delay.h"
#include "usart.h"
int main()
{
    test_neo();
}
void test_neo()
{
    DDRB = 0xFF;
    softuart_config(4, 3);
    softuart_enable_recv();
    usart_enable(MYUBRR);
    uint8_t data;
    while (1)
    { 
        data = softuart_read();
        usart_send(data);
        //_delay_ms(1000);
    }
}
