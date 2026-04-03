#ifndef __UART__H__
#define __UART__H__
#include <stdint.h>
#include "stm32f4xx.h"

void UART2_tx_init(void);
void uart2_write(int ch);

#endif
