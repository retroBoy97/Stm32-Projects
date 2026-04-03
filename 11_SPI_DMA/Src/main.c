#include <stdio.h>
#include <string.h>
#include "UART.h"
#include "DMA.h"
#include "UART_DMA.h"
#include "SPI_DMA.h"


extern uint8_t global_rx_complete;
extern uint8_t global_uart_complete;
extern char uart_data_buffer[UART_DATA_BUFF_SIZE];
char msg_buffer[120] = {'\0'};

int main(void){
    spi1_dma_init();

    while(1){
	printf("blabnla");
    }
}




