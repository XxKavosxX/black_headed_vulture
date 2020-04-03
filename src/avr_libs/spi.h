#ifndef __SPI_H__
#define __SPI_H__
#include "definitions.h"
#define CLK PB5  //Digital pin 13
#define MISO PB4 //Digital pin 12  DO
#define MOSI PB3 //Digital pin 11  DI
#define CS PB2   //Digital pin 10

void spi_enable();
uint8_t spi_send_recv(uint8_t data);


#endif