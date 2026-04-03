#include "DMA.h"
#include "stm32f4xx.h"

#define DMA2EN 		(1U << 22)	// DMA2 Enable bit in AHB1ENR
#define DMA_SCR_EN 	(1U << 0)	// DMA2 Enable bit in Stream Config Register
#define DMA_SCR_MIC 	(1U << 10)	// Memory Increment bit in Stream Config Register
#define DMA_SCR_PIC 	(1U << 9)	// Peripheral Increment bit in Stream Config Register
#define DMA_SCR_TCIE	(1U << 4)	// Transfer Complete interrupt enable in SCR
#define DMA_SCR_TEIE	(1U << 2)	// Transfer Error interrupt enable in SCR
#define DMA_SFCR_DMDIS	(1U << 2)	// Direct Mode Disable bit in  FIFO control register


void DMA2_Mem2Mem_Config(void){
    // Enable clock access to DMA module
    RCC -> AHB1ENR |= DMA2EN;

    // Disable DMA stream
    DMA2_Stream0 -> CR = 0;

    // Wait until stream is disabled
    while ((DMA2_Stream0 -> CR & DMA_SCR_EN)){}

    /****** Configure DMA parameters ******/
    // Set Memory data size to Half-Word
    DMA2_Stream0 -> CR |= (1U << 13);
    DMA2_Stream0 -> CR &= ~(1U << 14);

    // Set Peripheral data size to Half-Word
    DMA2_Stream0 -> CR |= (1U << 11);
    DMA2_Stream0 -> CR &= ~(1U << 12);

    // Enable Memory @ Increment
    DMA2_Stream0 -> CR |= DMA_SCR_MIC;

    // Enable Peripheral @ Increment
    DMA2_Stream0 -> CR |= DMA_SCR_PIC;

    // Select Mem to Mem transfer
    DMA2_Stream0 -> CR &= ~(1U << 6);
    DMA2_Stream0 -> CR |= (1U << 7);

    // Enable Transfer Complete Interrupt
    DMA2_Stream0 -> CR |= DMA_SCR_TCIE;

    // Enable Transfer Error Interrupt
    DMA2_Stream0 -> CR |= DMA_SCR_TEIE;

    // Disable direct mode
    DMA2_Stream0 -> FCR |= DMA_SFCR_DMDIS;

    // Set DMA FIFO threshhold
    DMA2_Stream0 -> FCR |= (1U << 0);
    DMA2_Stream0 -> FCR |= (1U << 1);

    // Enable DMA interrupt in NVIC
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void DMA2_Transfer_Start(uint32_t src_buff, uint32_t dest_buff, uint32_t len){
    // Set Peripheral @
    DMA2_Stream0 -> PAR = src_buff;

    // Set Memory @
    DMA2_Stream0 -> M0AR = dest_buff;

    // Set Transfer Length
    DMA2_Stream0 -> NDTR = len;

    // Enable DMA Stream
    DMA2_Stream0 -> CR |= DMA_SCR_EN;
}
