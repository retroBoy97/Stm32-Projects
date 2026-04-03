#include <stdio.h>
#include "UART.h"
#include "DMA.h"
#include "ADC_DMA.h"

void DMA2_Stream0_IRQHandler(void);

extern uint16_t adc_raw_data[NUM_OF_SAMPLES];

volatile uint8_t global_transfer_complete;

int main(void){

    global_transfer_complete = 0;

    UART2_tx_init();
    adc_tim_dma_init();

    while(1){
	if (global_transfer_complete){
	    global_transfer_complete = 0;
	    for(int i = 0; i < NUM_OF_SAMPLES; i++){
		printf("Sample number [%d] = %d \r\n", i, adc_raw_data[i]);
	    }
	    for(int j = 0; j < 90000; j++){}
	}
    }
}

void DMA2_Stream0_IRQHandler(void){
    // Check if Transfer Complete
    if((DMA2 -> LISR) & LISR_TCIF0){
	global_transfer_complete = 1;

	// Clear flag
	DMA2 -> LIFCR |= LIFCR_CTCIF0;
    }

}



