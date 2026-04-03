#include <stdio.h>
#include "UART.h"
#include "DMA.h"
#include "ADC_DMA.h"

extern uint16_t adc_raw_data[NUM_OF_CHANNELS];

int main(void){

    UART2_tx_init();
    adc_dma_init();

    while(1){

    }
}



