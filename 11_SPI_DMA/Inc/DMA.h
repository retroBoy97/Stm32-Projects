#ifndef __DMA_H__
#define __DMA_H__

#include "stm32f4xx.h"

#define LISR_TCIF0 	(1U << 5)   // Stream transfer complete interrupt flag in Low Interrupt Status Register
#define LIFCR_CTCIF0	(1U << 5)   // Stream clear transfer complete interrupt flag in Low Interrupt Flag Clear Register

#define LISR_TEIF0	(1U << 3)   // Stream transfer error interrupt flag in Low Interrupt Status Register
#define LIFCR_CTEIF0	(1U << 3)   // Stream clear transfer error interrupt flag in Low Interrupt Flag Clear Register

void DMA2_Transfer_Start(uint32_t src_buff, uint32_t dest_buff, uint32_t len);
void DMA2_Mem2Mem_Config(void);

#endif
