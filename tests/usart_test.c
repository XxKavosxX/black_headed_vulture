#include "usart.h"
#include "util/delay.h"
int main()
{
    usart_enable(MYUBRR);
    unsigned char data;
    DDRB = 0xFF;
    int8_t i= 48;
    while (1)
    {
        usart_write(usart_recv());
        
    }
}