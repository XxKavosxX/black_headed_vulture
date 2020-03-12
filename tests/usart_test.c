#include "usart.h"
#include "util/delay.h"
int main()
{
    usart_enable(MYUBRR);
    char data;
    DDRB = 0xFF;
    while (1)
    {
        data = usart_receive();
        usart_send(data);
        _delay_ms(10);
    }
}