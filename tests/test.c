#include "usart.h"
#include "util/delay.h"
int main()
{
    usart_enable(MYUBRR);
    usart_flush();
    uint8_t c=0;
    uint8_t N=10;
    while (1)
    {
        usart_send(c+48);
        c = ++c % N;
        _delay_ms(150);
    }
}