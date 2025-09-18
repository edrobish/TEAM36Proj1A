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
#define LED3 4
#define LED4 0
#define PORT1 GPIOA
#define PORT2 GPIOB
#define TIME 500
//Pins PA0, PA1, PA4 and PB0

void initialize(void) {
    RCC->AHB1ENR |= (1 << 0);
    RCC->AHB1ENR |= (1 << 1); // enables clock for GPIOs
    PORT1->MODER &= ~((0x3 << (LED1*2)) | (0x3 << (LED2*2)) | (0x3 << (LED3*2))); // resets these modes
    PORT1->MODER |= (0x1 << (LED1*2)) | (0x1 << (LED2*2)) | (0x1 << (LED3*2));  // sets these pins to GPIO
    PORT2->MODER &= ~((0x3 << (LED4*2)));
    PORT2->MODER |= (0x1 << (LED4*2));
}

void systick(void) {
    SysTick->LOAD = 15999;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}
/* 16 MHz clock, each tick 1/16000000 seconds*16000 = 1 ms, so this timer counts
from 15999 down to 0 per tick to simulate 1 ms*/

void delay(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) { }
    }
}
/*While loop spins until COUNTFLAG is 1, which indicates SysTick has elapsed 1 ms for every ms in
uint32_t ms
The  comparison pulls the COUNTFLAG bit out of the CTRL register*/

int main(void) {
    initialize();
    systick();
    while(1) { // first 16 bits enable a pin, the rest reset/disable
        PORT1->BSRR |= (1 << LED1);
        delay(TIME);
        PORT1->BSRR |= (1 << (LED1+16));
        PORT1->BSRR |= (1 << LED2);
        delay(TIME);
        PORT1->BSRR |= (1 << (LED2+16));
        PORT1->BSRR |= (1 << LED3);
        delay(TIME);
        PORT1->BSRR |= (1 << (LED3+16));
        PORT2->BSRR |= (1 << LED4);
        delay(TIME);
        PORT2->BSRR |= (1 << (LED4+16));
    }
}