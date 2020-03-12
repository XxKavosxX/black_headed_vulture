#include "soft_usart.h"
#include "util/delay.h"
#include "usart.h"
int main()
{
    test_recv();
}

void test_send(){
    set_rx();
    set_tx();
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
void test_recv(){

    set_rx();
    set_tx();
    enable_soft_usart_recv();
    usart_enable(MYUBRR);
    uint8_t data=0;
    while (1)
    {
        data = recv_char();
        usart_send(data);
        _delay_ms(10);
    }
}
