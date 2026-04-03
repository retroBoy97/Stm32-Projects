#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

char _a[1] = {'A'};
char* pa = _a;

char hello[] = {"Hello from Main"};
char* p_hello = hello;

int __io_putchar(int ch){
    ITM_SendChar(ch);
    return ch;
}

int main(void){

    printf("Hello \n");

    while(1){
    }
}
