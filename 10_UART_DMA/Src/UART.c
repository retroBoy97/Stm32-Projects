#include "UART.h"

#define UART2EN (1U << 17)
#define CR1_TE 	(1U << 3)	// Transmitter Enable bit in Control Register 1
#define CR1_RE 	(1U << 2)	// Receiver Enable bit in Control Register 1
#define CR1_UE	(1U << 13)	// USART Enable bit in Control Register
#define SR_TXE	(1U << 7) // Transmit Data Register

#define GPIOAEN (1U << 0)

#define UART_BAUDRATE 115200
#define CLK	      16000000

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
void uart2_write(int ch);

void UART2_tx_init(void){
    /******** Configure UART GPIO Pin ********/

    // Enable Clock Access to GPIOA
    RCC -> AHB1ENR |= GPIOAEN;

    // Set PA2 mode to alternate function mode
    GPIOA -> MODER &= ~(1U << 4);
    GPIOA -> MODER |= (1U << 5);

    // Set PA2 alternate function type to AF7 (UART2_TX)
    GPIOA -> AFR[0] |= (1U << 8);
    GPIOA -> AFR[0] |= (1U << 9);
    GPIOA -> AFR[0] |= (1U << 10);
    GPIOA -> AFR[0] &= ~(1U << 11);

    /******** Configure UART Module ********/
    // Enable Clock Access to UART2
    RCC -> APB1ENR |= UART2EN;

    // Set Baudrate
    uart_set_baudrate(CLK, UART_BAUDRATE);

    // Set Transfer Direction
    USART2 -> CR1 = CR1_TE;

    // Enable UART Module
    USART2 -> CR1 |= CR1_UE;

}

int __io_putchar(int ch){
    uart2_write(ch);
    return ch;
}

void uart2_write(int ch) {
    // Make sure the transmit data register is empty
    while (!(USART2 -> SR & SR_TXE)){}

    // Write to transmit data register
    USART2 -> DR = (ch & 0xFF);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate){
    return (periph_clk + (baudrate / 2U)) / baudrate;
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate){
    USART2 -> BRR = compute_uart_bd(periph_clk, baudrate);
}





