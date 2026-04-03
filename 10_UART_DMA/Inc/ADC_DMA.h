#ifndef __ADC_DMA_H__
#define __ADC_DMA_H__

#include<stdint.h>
#include "stm32f4xx.h"
#define NUM_OF_SAMPLES 10
#define NUM_OF_CHANNELS 2

#define LISR_TCIF0 	(1U << 5)   // Stream transfer complete interrupt flag in Low Interrupt Status Register
#define LIFCR_CTCIF0	(1U << 5)   // Stream clear transfer complete interrupt flag in Low Interrupt Flag Clear Register

void adc_dma_init(void);
void adc_tim_dma_init(void);

#endif
