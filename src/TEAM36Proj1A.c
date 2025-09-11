/****************************************************************
* TEAM 36: E. DROBISH and A. ANGELETAKIS
* CPEG222 Project1A, 9/10/25
* NucleoF466RE CMSIS STM32F446
****************************************************************/

#include "stm32f4xx.h"
#include <math.h>
#include <stdlib.h>

#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define PORT GPIOA
// LEDs are on pins PA0-PA5, but if we want to change this #define statements are easily modifiable

int main(void) {
    RCC->AHB1ENR |= (1 << 0);
    // set modes to 
    while(1) { // first 16 bits enable a pin, the rest reset/disable
        PORT->BSRR |= (1 << LED1);
        //delay
        PORT->BSRR |= (1 << (LED1+16));
        PORT->BSRR |= (1 << LED2);
        //delay
        PORT->BSRR |= (1 << (LED2+16));
        PORT->BSRR |= (1 << LED3);
        //delay
        PORT->BSRR |= (1 << (LED3+16));
        PORT->BSRR |= (1 << LED4);
        //delay
        PORT->BSRR |= (1 << (LED4+16));
    } // could make a helper function for on/off but probably unnecessary
}