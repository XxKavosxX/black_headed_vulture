#include <util/delay.h>
#include "sd_mmc.h"
#include "usart.h"
uint8_t write_msg[512];

int main()
{
   sd_test_write_read_block();
}
void sd_test_go_idle()
{
   usart_enable(MYUBRR);
   usart_flush();
   uint8_t stt;
   spi_enable();
   sd_mmc_power_up();
   stt = sd_mmc_go_idle();
   usart_send(stt + 48);
   while (1)
   {
   }
}
void sd_test_send_if()
{
   // array to hold responses
   uint8_t res[5];

   // initialize UART
   usart_enable(MYUBRR);

   // initialize SPI
   spi_enable();

   // start power up sequence
   sd_mmc_power_up();

   // command card to idle
   usart_write("Sending CMD0...\r\n");
   res[0] = sd_mmc_go_idle();
   usart_write("Response:\r\n");
   sd_print_r1(res[0]);

   // send if conditions
   usart_write("Sending CMD8...\r\n");
   sd_mmc_send_if_cond(res);
   usart_write("Response:\r\n");
   sd_print_r7(res);

   while (1)
      ;
}
void sd_test_read_ocr()
{
   // array to hold responses
   uint8_t res[5];

   // initialize UART
   usart_enable(MYUBRR);

   // initialize SPI
   spi_enable();

   // start power up sequence
   sd_mmc_power_up();

   // command card to idle
   usart_write("Sending CMD0...\r\n");
   res[0] = sd_mmc_go_idle();
   usart_write("Response:\r\n");
   sd_print_r1(res[0]);

   // send if conditions
   usart_write("Sending CMD8...\r\n");
   sd_mmc_send_if_cond(res);
   usart_write("Response:\r\n");
   sd_print_r7(res);

   // read ocr conditions
   usart_write("Sending CMD58...\r\n");
   sd_mmc_read_ocr(res);
   usart_write("Response:\r\n");
   sd_print_r3(res);

   while (1)
      ;
}
void sd_test_send_app_op()
{
   // array to hold responses
   uint8_t res[5];

   // initialize UART
   usart_enable(MYUBRR);

   // initialize SPI
   spi_enable();

   // start power up sequence
   sd_mmc_power_up();

   // command card to idle
   usart_write("Sending CMD0...\r\n");
   res[0] = sd_mmc_go_idle();
   usart_write("Response:\r\n");
   sd_print_r1(res[0]);

   // send if conditions
   usart_write("Sending CMD8...\r\n");
   sd_mmc_send_if_cond(res);
   usart_write("Response:\r\n");
   sd_print_r7(res);

   // send if conditions
   usart_write("Sending CMD58...\r\n");
   sd_mmc_read_ocr(res);
   usart_write("Response:\r\n");
   sd_print_r3(res);

   // command card to idle
   usart_write("Sending CMD55...\r\n");
   res[0] = sd_mmc_send_app();
   usart_write("Response:\r\n");
   sd_print_r1(res[0]);

   // command card to idle
   usart_write("Sending ACMD41...\r\n");
   res[0] = sd_mmc_send_op_cond();
   usart_write("Response:\r\n");
   sd_print_r1(res[0]);

   usart_write("Unrecognized command\r\n");
   while (1)
      ;
}
void sd_test_read_block()
{
   usart_write("SD card read block\n");

   // array to hold responses
   uint8_t res[5], sdBuf[512], token;

   // initialize UART
   usart_enable(MYUBRR);

   // initialize SPI
   spi_enable();

   // initialize sd card
   if (sd_mmc_init() != 0)
   {
      usart_write("Error initializaing SD CARD\r\n");
      while (1)
         ;
   }
   else
   {
      usart_write("SD Card initialized\r\n");
   }

   // read sector 0
   res[0] = sd_mmc_read_block(0x00000800, sdBuf, &token);

   // print response
   if (SD_R1_NO_ERROR(res[0]) && (token == 0xFE))
   {
      for (uint16_t i = 0; i < 512; i++)
         usart_write(int2hex(sdBuf[i]));
      usart_write("\r\n");
   }
   else
   {
      usart_write("Error reading sector\r\n");
   }

   while (1)
      ;
}
void sd_test_write_block()
{
   usart_write("SD card write block\n");

   // array to hold responses
   uint8_t res, buf[512], token;

   // initialize UART
   usart_enable(MYUBRR);

   // initialize SPI
   spi_enable();

   // initialize sd card
   if (sd_mmc_init() != 0)
   {
      usart_write("Error initializaing SD CARD\r\n");
      while (1)
         ;
   }
   else
   {
      usart_write("SD Card initialized\r\n");
   }

   // fill buffer with 0x55
   for (uint16_t i = 0; i < 512; i++)
      buf[i] = 0x23;

   // write 0x55 to address 0x100 (256)
   res = sd_mmc_write_block(0x00000800, buf, &token);
   if (res)
   {
      usart_write("Block writed\r\n");
   }
}
void sd_test_write_read_block()
{
   usart_write("SD card write block\n");

   // array to hold responses
   uint8_t res[5], buf[512], token;

   // initialize UART
   usart_enable(MYUBRR);

   // initialize SPI
   spi_enable();

   // initialize sd card
   if (sd_mmc_init() != 0)
   {
      usart_write("Error initializaing SD CARD\r\n");
      while (1)
         ;
   }
   else
   {
      usart_write("SD Card initialized\r\n");
   }

   // read sector 0
   res[0] = sd_mmc_read_block(0x00000800, buf, &token);

   // print response
   if (SD_R1_NO_ERROR(res[0]) && (token == 0xFE))
   {
      for (uint16_t i = 0; i < 512; i++)
         usart_write(int2hex(buf[i]));
      usart_write("\r\n");
   }
   else
   {
      usart_write("Error reading sector\r\n");
   }

   // fill buffer with 0x55
   for (uint16_t i = 0; i < 512; i++)
      buf[i] = 0x23;

   // write 0x55 to address 0x100 (256)
   res[0] = sd_mmc_write_block(0x00000800, buf, &token);
   if (res[0])
   {
      usart_write("Block writed\r\n");
   }
}

void sd_test_out_of_range_data()
{
   uint8_t res, buf[512], token, error;
   res = sd_mmc_read_block(0xffffffff, buf, &token);

   usart_write("Response 1:\r\n");
   sd_print_r1(res);

   // if error token received
   if (!(token & 0xF0))
   {
      usart_write("Error token:\r\n");
      sd_print_data_token(error);
   }
   else if (token == 0xFF)
   {
      usart_write("Timeout\r\n");
   }
}