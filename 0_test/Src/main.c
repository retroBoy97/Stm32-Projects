#include "stm32f4xx.h"

#define GPIOAEN (1U << 0)
#define PIN5 	(1U << 5) // Bit number 5 set to 1
#define LED_PIN	PIN5

int main(void){
    // Enable clock acces to GPIOA
    RCC -> AHB1ENR |= GPIOAEN;

    // Set PA5 to Output Mode
    GPIOA -> MODER |= (1U << 10);
    GPIOA -> MODER &= ~(1U << 11);

    while(1){
    	GPIOA -> ODR ^= LED_PIN;
    	for (int i = 0; i < 1000000; i++){}
    }
}
