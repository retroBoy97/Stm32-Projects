#include "stm32f4xx.h"

char _a[1] = {'A'};
char* pt_a = _a;

int total = 0;

int main(void){

    for (int i = 0; i < 10; i++){
	total += i;
    }

    while(1){
	ITM_sendChar(*pt_a);
    }
}
