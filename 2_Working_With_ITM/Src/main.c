#include "stm32f4xx.h"
#include <string.h>

char _a[1] = {'A'};
char* pa = _a;

char hello[] = {"Hello from Main"};
char* p_hello = hello;

int main(void){

    for (int i = 0; i < strlen(hello); i++){
	ITM_SendChar(*p_hello++);
    }

    while(1){
    }
}
