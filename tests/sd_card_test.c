#include <util/delay.h>
#include "sd_mmc.h"
#include "usart.h"
int main()
{
   sd_read_info_test();
}
void sd_read_info_test()
{
   _delay_ms(1000);
   usart_enable(MYUBRR);
   sd_mmc_enable();
   //usart_write("Hello");
   uint8_t stt = sd_mmc_start();
   switch (stt)
   {
   case 0:
      usart_write("OK");
   case 1:
      usart_write("ERROR: don't GO_IDLE_MODE");
      break;
   case 2:
      usart_write("ERROR: don't SEND_OP_MODE");
      break;
   default:
      usart_write("ERROR: other value");
      break;
   }
   while (1)
   {
   }
}