#include "UART_DMA.h"

/***** UART Config *****/
#define UART2EN (1U << 17)	// UART2 Enable bit in APB1ENR
#define CR1_TE 	(1U << 3)	// Transmitter Enable bit in Control Register 1
#define CR1_RE 	(1U << 2)	// Receiver Enable bit in CR1
#define CR1_UE	(1U << 13)	// USART Enable bit in CR1
#define SR_TXE	(1U << 7) 	// Transmit Data Register Empty or not
#define CR3_DMAT (1U << 7)	// DMA Transmitter Enable bit in CR3
#define CR3_DMAR (1U << 6)	// DMA Receiver Enable bit in CR3
#define SR_TC	(1U << 6)	// Transmission Complete bit in Status Register
#define CR1_TCIE (1U << 6)	// Transmission complete interrupt enable bit in CR1

/***** GPIO A *****/
#define GPIOAEN (1U << 0)

#define UART_BAUDRATE 115200
#define CLK	      16000000


/***** DMA *****/
// Config
#define DMA1EN (1U << 21)	// DMA1 Enable bit in AHB1ENR
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

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
void uart2_write(int ch);

char uart_data_buffer[UART_DATA_BUFF_SIZE];
uint8_t global_rx_complete;
uint8_t global_uart_complete;

void uart2_rx_tx_init(void){
    /******** Configure UART GPIO Pin ********/

    // Enable Clock Access to GPIOA
    RCC -> AHB1ENR |= GPIOAEN;

    // Set PA2 mode to alternate function mode
    GPIOA -> MODER &= ~(1U << 4);
    GPIOA -> MODER |= (1U << 5);

    // Set PA3 mode to alternate function mode
    GPIOA -> MODER &= ~(1U << 6);
    GPIOA -> MODER |= (1U << 7);

    // Set PA2 alternate function type to AF7 (UART2_TX)
    GPIOA -> AFR[0] |= (1U << 8);
    GPIOA -> AFR[0] |= (1U << 9);
    GPIOA -> AFR[0] |= (1U << 10);
    GPIOA -> AFR[0] &= ~(1U << 11);

    // Set PA3 alternate function type to AF7 (UART2_RX)
    GPIOA -> AFR[0] |= (1U << 12);
    GPIOA -> AFR[0] |= (1U << 13);
    GPIOA -> AFR[0] |= (1U << 14);
    GPIOA -> AFR[0] &= ~(1U << 15);

    /******** Configure UART Module ********/
    // Enable Clock Access to UART2
    RCC -> APB1ENR |= UART2EN;

    // Set Baudrate
    uart_set_baudrate(CLK, UART_BAUDRATE);

    // Select to use DMA for TX and RX
    USART2 -> CR3 = CR3_DMAT | CR3_DMAR;

    // Set Transfer Direction
    USART2 -> CR1 = CR1_TE | CR1_RE;

    // Clear TC flag
    USART2 -> SR &= ~(SR_TC);

    // Enable TCIE
    USART2 -> CR1 |= CR1_TCIE;

    // Enable UART Module
    USART2 -> CR1 |= CR1_UE;

    // Enable USART2 Interrupt in the NVIC
    NVIC_EnableIRQ(USART2_IRQn);
}

void DMA1_Init(void){
    // Enable Clock access to DMA1
    RCC -> AHB1ENR |= DMA1EN;

    // Enable DMA1 Stream 6 Intrrupt in NVIC
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void DMA1_Stream5_UART_RX_Config(void){
    // Disable DMA stream
    DMA1_Stream5 -> CR &= ~DMA_SCR_EN;

    // Wait till DMA is Disabled
    while ((DMA1_Stream5 -> CR) & DMA_SCR_EN){}

    // Clear Interrupt flags for stream 5
    DMA1 -> HIFCR = HIFCR_CDMEIF5 | HIFCR_CTEIF5 | HIFCR_CTCIF5;

    // Set Periph @
    DMA1_Stream5 -> PAR = (uint32_t) (&(USART2 -> DR));

    // Set Mem @
    DMA1_Stream5 -> M0AR = (uint32_t)(&uart_data_buffer);

    // Set number of transfer
    DMA1_Stream5 -> NDTR = (uint16_t) UART_DATA_BUFF_SIZE;

    // Select Channel 4
    DMA1_Stream5 -> CR &= ~(1U << 25);
    DMA1_Stream5 -> CR &= ~(1U << 26);
    DMA1_Stream5 -> CR |= (1U << 27);

    // Enable Mem @ Increment
    DMA1_Stream5 -> CR |= DMA_SCR_MIC;

    // Enable Transfer Complete Interrupt
    DMA1_Stream5 -> CR |= DMA_SCR_TCIE;

    // Enable Circular mode
    DMA1_Stream5 -> CR |= DMA_SCR_CIRC;

    // Set Transfer direction : Periph to Mem
    DMA1_Stream5 -> CR  &= ~(1U << 6);
    DMA1_Stream5 -> CR  &= ~(1U << 7);

    // Enable DMA Stream
    DMA1_Stream5 -> CR |= DMA_SCR_EN;

    // Enable DMA Stream5 Interrupt in NVIC
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

void DMA1_Stream6_UART_TX_Config(uint32_t msg_to_send, uint32_t msg_len){
    // Disable DMA stream
    DMA1_Stream6 -> CR &= ~DMA_SCR_EN;

    // Wait till DMA is Disabled
    while ((DMA1_Stream6 -> CR) & DMA_SCR_EN){}

    // Clear Interrupt flags for Stream 6
    DMA1 -> HIFCR = HIFCR_CDMEIF6 | HIFCR_CTEIF6 | HIFCR_CTCIF6;

    // Set Periph @
    DMA1_Stream6 -> PAR = (uint32_t) (&(USART2 -> DR));

    // Set Mem @
    DMA1_Stream6 -> M0AR = msg_to_send;

    // Set number of transfer
    DMA1_Stream6 -> NDTR = msg_len;

    // Select Channel 4
    DMA1_Stream6 -> CR &= ~(1U << 25);
    DMA1_Stream6 -> CR &= ~(1U << 26);
    DMA1_Stream6 -> CR |= (1U << 27);

    // Enable Mem @ Increment
    DMA1_Stream6 -> CR |= DMA_SCR_MIC;

    // Set Transfer direction : Mem to Periph
    DMA1_Stream6 -> CR  |= (1U << 6);
    DMA1_Stream6 -> CR  &= ~(1U << 7);

    // Set Transfer Complete Interrupt
    DMA1_Stream6 -> CR |= DMA_SCR_TCIE;

    // Enable DMA Stream
    DMA1_Stream6 -> CR |= DMA_SCR_EN;
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate){
    return (periph_clk + (baudrate / 2U)) / baudrate;
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate){
    USART2 -> BRR = compute_uart_bd(periph_clk, baudrate);
}

void DMA1_Stream6_IRQHandler(void){
    if((DMA1 -> HISR) & HIFSR_TCIF6){
	// Do something

	// Clear flag
	DMA1 -> HIFCR |= HIFCR_CTCIF6;
    }
}

void DMA1_Stream5_IRQHandler(void){
    if((DMA1 -> HISR) & HIFSR_TCIF5){
	global_rx_complete = 1;

	// Do something

	// Clear flag
	DMA1 -> HIFCR |= HIFCR_CTCIF5;
    }
}

void USART2_IRQHandler(void){
    global_uart_complete = 1;

    // clear TC Interrupt flag
    USART2 -> SR &= ~SR_TC;
}
