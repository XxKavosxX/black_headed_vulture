#include "usart.h"
#include "util/delay.h"
int main()
{
    usart_test_send();
}
void usart_test_recv()
{
    usart_enable(MYUBRR);
    unsigned char data;
    DDRB = 0xFF;
    int8_t i = 48;
    while (1)
    {
        usart_write(usart_recv());
    }
}
void usart_test_send()
{
    usart_enable(MYUBRR);
    unsigned char data;
    DDRB = 0xFF;
    int8_t i = 48;
    while (1)
    {
        usart_send('a');
        _delay_ms(100);
    }
}