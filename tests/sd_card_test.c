#include <util/delay.h>
#include "sd_mmc.h"
#include "usart.h"
int main()
{
   sd_read_info_test();
}
void sd_read_info_test()
{
   usart_enable(MYUBRR);
   sd_mmc_enable();

   uint8_t stt = sd_mmc_start();
   if (stt == 0)
   {
      usart_write("\n OK \n");
   }
   else if (stt == 1)
   {
      usart_write("\n ERROR: don't GO_IDLE_MODE\n ");
   }
   else if (stt == 2)
   {
      usart_write("\n ERROR: don't SEND_OP_MODE \n");
   }
   else
   {
      usart_write("\n ERROR: other error \n");
   }
   while (1)
   {
   }
}