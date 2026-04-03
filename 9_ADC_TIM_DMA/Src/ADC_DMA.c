#include "ADC_DMA.h"

#define GPIOAEN		(1U << 0)

#define ADC1EN 		(1U << 8)	// ADC1 Enable bit in APB1
#define CR1_SCAN	(1U << 6)	// Scan Enable bit in ADC Control register 1
#define CR2_DMA 	(1U << 8)	// DMA Enable bit in ADC CR2
#define CR2_DDS		(1U << 9)	// DMA Disable Selection bit in CR2
#define CR2_CONT	(1U << 1)	// Continuous Conversion bit in CR2
#define CR2_ADCON	(1U << 0)	// ADC Converter ON / OFF bit in CR2
#define CR2_SWSTART	(1U << 30)	// ADC

#define DMA2EN 		(1U << 22)	// DMA2 Enable bit in AHB1ENR
#define DMA_SCR_EN 	(1U << 0)	// DMA2 Enable bit in Stream Config Register
#define DMA_SCR_MIC 	(1U << 10)	// Memory Increment bit in Stream Config Register
#define DMA_SCR_PIC 	(1U << 9)	// Peripheral Increment bit in Stream Config Register
#define DMA_SCR_CIRC	(1U << 8)	// Circular Mode Enable bit in Stream Config Register
#define DMA_SCR_TCIE	(1U << 4)	// Transfer Complete interrupt enable in SCR
#define DMA_SCR_TEIE	(1U << 2)	// Transfer Error interrupt enable in SCR
#define DMA_SFCR_DMDIS	(1U << 2)	// Direct Mode Disable bit in  FIFO control register

#define TIM2EN 		(1U << 0)
#define CR1_CEN		(1U << 0)

uint16_t adc_raw_data[NUM_OF_SAMPLES];


void adc_dma_init(void){
    /***** GPIO Config *****/
    // Enable Clock access to ADC GPIO Pin's Port
    RCC -> AHB1ENR |= GPIOAEN;

    // Set PA0 and PA1 to Analog mode
    GPIOA -> MODER |= (1U << 0);
    GPIOA -> MODER |= (1U << 1);

    GPIOA -> MODER |= (1U << 2);
    GPIOA -> MODER |= (1U << 3);

    /***** ADC Config *****/
    // Enable Clock access to ADC
    RCC -> APB2ENR |= ADC1EN;

    // Set sequence length
    ADC1 -> SQR1 |= (1U << 20);
    ADC1 -> SQR1 &= ~(1U << 21);
    ADC1 -> SQR1 &= ~(1U << 22);
    ADC1 -> SQR1 &= ~(1U << 23);

    // Set Sequence
    ADC1 -> SQR3 = (0U << 0) | (1U << 5);

    // Enable Scan mode
    ADC1 -> CR1 = CR1_SCAN;

    // Select to use DMA
    ADC1 -> CR2 |= CR2_CONT | CR2_DMA | CR2_DDS;

    /***** DMA Config *****/
    // Enable clock access to DMA2
    RCC -> AHB1ENR |= DMA2EN;

    // Disable DMA stream
    DMA2_Stream0 -> CR &= ~DMA_SCR_EN;

    // Wait till DMA is Disabled
    while ((DMA2_Stream0 -> CR & DMA_SCR_EN)){}

    // Enable Circular Mode
    DMA2_Stream0 -> CR |= DMA_SCR_CIRC;

    // Set Memory data size to Half-Word
    DMA2_Stream0 -> CR |= (1U << 13);
    DMA2_Stream0 -> CR &= ~(1U << 14);

    // Set Peripheral data size to Half-Word
    DMA2_Stream0 -> CR |= (1U << 11);
    DMA2_Stream0 -> CR &= ~(1U << 12);

    // Enable Memory @ Increment
    DMA2_Stream0 -> CR |= DMA_SCR_MIC;

    // Set Periph @
    DMA2_Stream0 -> PAR = (uint32_t) (&(ADC1 -> DR));

    // Set Mem @
    DMA2_Stream0 -> M0AR = (uint32_t)(&adc_raw_data);

    // Set number of transfer
    DMA2_Stream0 -> NDTR = (uint16_t) NUM_OF_CHANNELS;

    // Enable DMA Stream
    DMA2_Stream0 -> CR |= DMA_SCR_EN;

    /***** ADC Config *****/
    // Enable ADC
    ADC1 -> CR2 |= CR2_ADCON;

    // Start ADC
    ADC1 -> CR2 |= CR2_SWSTART;
}

void adc_tim_dma_init(void){
    /***** GPIO Config *****/
    // Enable Clock access to ADC GPIO Pin's Port
    RCC -> AHB1ENR |= GPIOAEN;

    // Set PA0 Analog mode
    GPIOA -> MODER |= (1U << 0);
    GPIOA -> MODER |= (1U << 1);

    /***** ADC Config *****/
    // Enable Clock access to ADC
    RCC -> APB2ENR |= ADC1EN;

    // Select to use DMA
    ADC1 -> CR2 |= CR2_DMA | CR2_DDS;

    // Select External Trigger on rising edge
    ADC1 -> CR2 |= (1U << 28);
    ADC1 -> CR2 &= ~(1U << 29);

    // Select Timer 2 TRGO event
    ADC1 -> CR2 &= ~(1U << 24);
    ADC1 -> CR2 |= (1U << 25);
    ADC1 -> CR2 |= (1U << 26);
    ADC1 -> CR2 &= ~(1U << 27);

    /***** DMA Config *****/
    // Enable clock access to DMA2
    RCC -> AHB1ENR |= DMA2EN;

    // Disable DMA stream
    DMA2_Stream0 -> CR &= ~DMA_SCR_EN;

    // Wait till DMA is Disabled
    while ((DMA2_Stream0 -> CR & DMA_SCR_EN)){}

    // Enable Circular Mode
    DMA2_Stream0 -> CR |= DMA_SCR_CIRC;

    // Set Memory data size to Half-Word
    DMA2_Stream0 -> CR |= (1U << 13);
    DMA2_Stream0 -> CR &= ~(1U << 14);

    // Set Peripheral data size to Half-Word
    DMA2_Stream0 -> CR |= (1U << 11);
    DMA2_Stream0 -> CR &= ~(1U << 12);

    // Enable Memory @ Increment
    DMA2_Stream0 -> CR |= DMA_SCR_MIC;

    // Enable Transfer Complete Interrupt
    DMA2_Stream0 -> CR |= DMA_SCR_TCIE;

    // Set Periph @
    DMA2_Stream0 -> PAR = (uint32_t) (&(ADC1 -> DR));

    // Set Mem @
    DMA2_Stream0 -> M0AR = (uint32_t)(&adc_raw_data);

    // Set number of transfer
    DMA2_Stream0 -> NDTR = (uint16_t) NUM_OF_SAMPLES;

    // Enable DMA interrupt in NVIC
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);

    /***** Timer Config *****/
    // Config Timer prd to 100 Hz => ADC samlpes every 10ms (100Hz rate)
    // Enable Clock Access to TIM2
    RCC -> APB1ENR |= TIM2EN;

    // Set TIM Prescaler value
    TIM2 -> PSC = 16000 - 1; // 16 000 000 / 16 000 = 1000

    // Set TIM Auto-Reload value
    TIM2 -> ARR = 10 - 1; // 1 000 / 10 = 100Hz

    // Configure Master Mode Selection bits
    TIM2 -> CR2 &= ~ (1U << 4);
    TIM2 -> CR2 |= (1U << 5);
    TIM2 -> CR2 &= ~ (1U << 6);

    /***** Enable Modules *****/
    // Enable ADC
    ADC1 -> CR2 |= CR2_ADCON;

    // Enable DMA Stream
    DMA2_Stream0 -> CR |= DMA_SCR_EN;

    // Enable Timer
    TIM2 -> CR1 |= CR1_CEN;

}
