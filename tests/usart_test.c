#include "usart.h"
#include "util/delay.h"
int main(){
    while(1){
    usart_enable(MYUBRR);
    usart_write_flash(PSTR("Hello USART!"));
    _delay_ms(1000);
    }
}