#include <stdio.h>
#include "UART.h"
#include "DMA.h"
#include "ADC_DMA.h"
#include "UART_DMA.h"
#include <string.h>

extern uint8_t global_rx_complete;
extern uint8_t global_uart_complete;
extern char uart_data_buffer[UART_DATA_BUFF_SIZE];
char msg_buffer[120] = {'\0'};

int main(void){
    global_uart_complete = 0;
    global_rx_complete = 0;

    uart2_rx_tx_init();
    DMA1_Init();
    DMA1_Stream5_UART_RX_Config();
    sprintf(msg_buffer, "Initialisation \n\r");
    DMA1_Stream6_UART_TX_Config((uint32_t)msg_buffer, (uint32_t)(strlen(msg_buffer)));

    while(!global_uart_complete){}

    while(1){
	if(global_rx_complete){
	    sprintf(msg_buffer, "Message Received : %s \r\n", uart_data_buffer);
	    global_rx_complete = 0;
	    DMA1_Stream6_UART_TX_Config((uint32_t)msg_buffer, (uint32_t)(strlen(msg_buffer)) );
	    while(! global_uart_complete){}
	}
    }
}




