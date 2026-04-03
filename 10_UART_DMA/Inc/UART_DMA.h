#ifndef __UART_DMA_H__
#define __UART_DMA_H__
#include <stdint.h>
#include "stm32f4xx.h"

#define UART_DATA_BUFF_SIZE 5

void uart2_rx_tx_init(void);
void DMA1_Init(void);
void DMA1_Stream5_UART_RX_Config(void);
void DMA1_Stream6_UART_TX_Config(uint32_t msg_to_send, uint32_t msg_len);
#endif
