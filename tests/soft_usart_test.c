#include "soft_usart.h"
#include "util/delay.h"
int main()
{
    set_rx();
    set_tx();
    while (1)
    {
       send_char('A');
       _delay_ms(1000);
    }
}