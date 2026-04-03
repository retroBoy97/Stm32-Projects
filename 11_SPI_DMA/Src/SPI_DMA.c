#include "spi_dma.h"

/****** GPIOA ******/
#define GPIOAEN (1U << 0)

/****** SPI1 Config *****/
#define SPI1EN		(1U << 12)
#define CR1_SSM 	(1U << 9)	// SSM bit in CR1
#define CR1_SSI		(1U << 8)	// SSI bit in CR1
#define CR1_MSTR	(1U << 2)	// Master selection bit in SPI CR1
#define CR1_CPHA	(1U << 1)	// Clock Phase bit in CR1
#define CR1_CPOL	(1U << 0)	// Clock Polarity bit in CR1
#define CR2_TXDMAEN 	(1U << 1)	// Tx buffer DMA enable bit in CR2
#define CR2_RXDMAEN	(1u << 0)	// Rx buffer DMA enable bit in CR2
#define CR1_SPE		(1U << 6)	// SPI enable bit in CR1

/***** DMA Config *****/
#define DMA2EN		(1U << 22)	// DMA2 Enable bit in AHB1ENR
#define DMA_SCR_EN 	(1U << 0)	// DMA2 Enable bit in Stream Config Register
#define DMA_SCR_MIC 	(1U << 10)	// Memory Increment bit in Stream Config Register
#define DMA_SCR_PIC 	(1U << 9)	// Peripheral Increment bit in Stream Config Register
#define DMA_SCR_CIRC	(1U << 8)	// Circular Mode Enable bit in Stream Config Register
#define DMA_SCR_TCIE	(1U << 4)	// Transfer Complete interrupt enable in SCR
#define DMA_SCR_TEIE	(1U << 2)	// Transfer Error interrupt enable in SCR
#define DMA_SFCR_DMDIS	(1U << 2)	// Direct Mode Disable bit in  FIFO control register

// DMA Interrupts
#define HIFCR_CDMEIF5	(1U << 8)	// Clear Direct mode error interrupt flag bit in HIFCR
#define HIFCR_CTEIF5	(1U << 9)	// Clear Transfer Error Interrupt Flag bit in HIFCR
#define HIFCR_CTCIF5	(1U << 11)	// Clear Transfer Complete Interrupt Flag in HIFCR

#define HIFCR_CDMEIF6	(1U << 18)	// Clear Direct mode error interrupt flag bit in HIFCR
#define HIFCR_CTEIF6	(1U << 19)	// Clear Transfer Error Interrupt Flag bit in HIFCR
#define HIFCR_CTCIF6	(1U << 21)	// Clear Transfer Complete Interrupt Flag in HIFCR

#define HIFSR_TCIF5	(1U << 11)	// Transfer Complete Interrupt flag in High Interrupt status register
#define HIFSR_TCIF6	(1U << 21)

void spi1_dma_init(){

    /***** GPIO Config *****/
    // Enable Clock access to GPIOA
    RCC -> AHB1ENR |= GPIOAEN;

    // Set PA5, PA6, PA7 to AF Mode

    // PA5
    GPIOA -> MODER &= ~(1U << 10);
    GPIOA -> MODER |= (1 << 11);

    // PA6
    GPIOA -> MODER &= ~(1U << 12);
    GPIOA -> MODER |= (1 << 13);

    // PA7
    GPIOA -> MODER &= ~(1U << 14);
    GPIOA -> MODER |= (1 << 15);

    // Select alternate type to AF5

    // PA5 : SPI1_SCK
    GPIOA -> AFR[0] |= (1 << 20);
    GPIOA -> AFR[0] &= ~(1 << 21);
    GPIOA -> AFR[0] |= (1 << 22);
    GPIOA -> AFR[0] &= ~(1 << 23);

    // PA6 : SPI1_MISO
    GPIOA -> AFR[0] |= (1 << 24);
    GPIOA -> AFR[0] &= ~(1 << 25);
    GPIOA -> AFR[0] |= (1 << 26);
    GPIOA -> AFR[0] &= ~(1 << 27);

    // PA7 : SPI1_MOSI
    GPIOA -> AFR[0] |= (1 << 28);
    GPIOA -> AFR[0] &= ~(1 << 29);
    GPIOA -> AFR[0] |= (1 << 30);
    GPIOA -> AFR[0] &= ~(1 << 31);


    /***** SPI Config *****/
    // Enable Clock Access to SPI1 module
    RCC -> APB2ENR |= SPI1EN;

    // Set Software Slave management
    SPI1 -> CR1 |= CR1_SSM | CR1_SSI;

    // Set SPI to Master mode
    SPI1 -> CR1 |= CR1_MSTR;

    // Set CPHA and CPOL
    SPI1 -> CR1 |= SPI_CR1_BR_0 | CR1_CPHA | CR1_CPOL;

    // Set Clock divider : PCLK / 4
    SPI1 -> CR1 |= (1U << 3);
    SPI1 -> CR1 &= ~(1U << 4);
    SPI1 -> CR1 &= ~(1U << 5);

    // Select to use DMA
    SPI1 -> CR2 |= CR2_TXDMAEN | CR2_RXDMAEN;

    // Enable SPI
    SPI1 -> CR1 |= CR1_SPE;
}

void dma2_stream3_spi_tx_init(void){

    /***** DMA Config *****/
    // Enable clock Access to DMA
    RCC -> AHB1ENR |= DMA2EN;

    // Disable DMA Stream
    DMA2_Stream3 -> SCR = ~DMA_SCR_EN;

    // Wait till stream is disabled
    while ((DMA2_Stream3 -> CR & DMA_SCR_EN)){}

    /***** Configure DMA Stream Parameters *****/
    // Enable Memory @ Increment
    DMA2_Stream3 -> CR |= DMA_SCR_MIC;

    // Set Transfer direction : Mem to Periph
    DMA2_Stream3 -> CR |= (1U << 6);
    DMA2_Stream3 -> CR &= ~(1U << 7);

    // Set Transfer Complete interrupt
    DMA2_Stream3 -> CR |= DMA_SCR_TCIE;

    // Enable Transfer error Interrupt
    DMA2_Stream3 -> CR |= DMA_SCR_TEIE;

    // Disable direct mode
    DMA2_Stream3 -> FCR |= DMA_SFCR_DMDIS;

    // Set DMA FIFO threshold
    DMA2_Stream3 -> FCR |= (1U << 0);
    DMA2_Stream3 -> FCR |= (1U << 1);

    // Enable DMA Interrupt in the NVIC
    NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

void dma2_stream2_spi_rx_init(void){

    /***** DMA Config *****/
    // Enable clock Access to DMA
    RCC -> AHB1ENR |= DMA2EN;

    // Disable DMA Stream
    DMA2_Stream2 -> SCR = ~DMA_SCR_EN;

    // Wait till stream is disabled
    while ((DMA2_Stream2 -> CR & DMA_SCR_EN)){}

    /***** Configure DMA Stream Parameters *****/
    // Enable Memory @ Increment
    DMA2_Stream2 -> CR |= DMA_SCR_MIC;

    // Set Transfer direction : Periph to Mem
    DMA2_Stream2 -> CR &= ~(1U << 6);
    DMA2_Stream2 -> CR &= ~(1U << 7);

    // Set Transfer Complete interrupt
    DMA2_Stream2 -> CR |= DMA_SCR_TCIE;

    // Enable Transfer error Interrupt
    DMA2_Stream2 -> CR |= DMA_SCR_TEIE;

    // Disable direct mode
    DMA2_Stream2 -> FCR |= DMA_SFCR_DMDIS;

    // Set DMA FIFO threshold : Full
    DMA2_Stream2 -> FCR |= (1U << 0);
    DMA2_Stream2 -> FCR |= (1U << 1);

    // Enable DMA Interrupt in the NVIC
    NVIC_EnableIRQ(DMA2_Stream2_IRQn);




}
