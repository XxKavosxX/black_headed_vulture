#include "soft_usart.h"
#include "util/delay.h"
#include "usart.h"
int main()
{
    test_read();
}

void test_send()
{
    softuart_config(2, 3);
    while (1)
    {
        send_char('A');
        _delay_ms(1000);
        send_char('B');
        _delay_ms(1000);
        send_char('C');
        _delay_ms(1000);
    }
}
void test_recv()
{

    softuart_config(2, 3);
    softuart_enable_recv();
    usart_enable(MYUBRR);
    uint8_t data = 0;
    while (1)
    {
        usart_send(data);
        _delay_ms(10);
    }
}
void test_read()
{

    softuart_config(2, 3);
    softuart_enable_recv();
    usart_enable(MYUBRR);
    uint8_t d = 0;
    while (1)
    {
        usart_write(softuart_read());
        _delay_ms(500);
    }
}

void test_write()
{

    softuart_config(2, 3);
    softuart_enable_recv();
    usart_enable(MYUBRR);
    uint8_t d = 0;
    while (1)
    {
        softuart_write("Hello Softuart!");
        _delay_ms(500);
    }
}