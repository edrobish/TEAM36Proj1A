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
#define BTN 13
#define PORT1 GPIOA
#define PORT2 GPIOB
#define PORTBTN GPIOC
#define TIME 250
//Pins PA0, PA1, PA4 and PB0

volatile uint32_t ms = 0;
volatile uint8_t run_flag = 1;
volatile int direction = 1;


void initialize_leds(void) {
    RCC->AHB1ENR |= (1 << 0);
    RCC->AHB1ENR |= (1 << 1);  // enables clock for GPIOs
    PORT1->MODER &= ~((0x3 << (LED1*2)) | (0x3 << (LED2*2)) | (0x3 << (LED3*2))); // resets these modes
    PORT1->MODER |= (0x1 << (LED1*2)) | (0x1 << (LED2*2)) | (0x1 << (LED3*2));  // sets these pins to GPIO
    PORT2->MODER &= ~((0x3 << (LED4*2)));
    PORT2->MODER |= (0x1 << (LED4*2));
}

void initialize_systick(void) {
    SysTick->LOAD = 15999;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}
/* 16 MHz clock, each tick 1/16000000 seconds*16000 = 1 ms, so this timer counts
from 15999 down to 0 per tick to simulate 1 ms*/

void initialize_btn(void) {
    RCC->AHB1ENR |= (1 << 2);
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    PORTBTN->MODER &= ~(0x3 << (BTN*2)); // same as leds
    PORTBTN->PUPDR &= ~(0x3 << (BTN*2));
    PORTBTN->PUPDR |= (0x1 << (BTN*2)); // clear and set pull-up
    EXTI->IMR |= (1 << BTN);
    EXTI->FTSR |= (1 << BTN);
    SYSCFG->EXTICR[3] &= ~(0xF << 4);
    SYSCFG->EXTICR[3] |= (0x2 << 4); // clear and map interrupt
    EXTI->PR |= (1 << BTN); // so it doesn't float and fire erroneously
    NVIC_SetPriority(EXTI15_10_IRQn, 0);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void SysTick_Handler(void) {
    ms++;
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << BTN)) {
        EXTI->PR |= (1 << BTN);
        run_flag = !run_flag;
        if (!run_flag) {
            direction = -direction;
        }
    }
}

int main(void) {
    initialize_leds();
    initialize_systick();
    initialize_btn();
    uint32_t leds[4] = {LED1,LED2,LED3,LED4}, i = 0, prev_i = 4;
    while(1) { // first 16 bits enable a pin, the rest reset/disable
        if (run_flag && ms >= TIME) {
            ms -= TIME;
            if (prev_i != 4) {
                if (prev_i < 3) {
                    PORT1->BSRR |= (1 << (leds[prev_i]+16));
                }
                else {
                    PORT2->BSRR |= (1 << (leds[prev_i]+16));
                }
            }
            if (i < 3) {
                PORT1->BSRR |= (1 << leds[i]);
                }
            else {
                PORT2->BSRR |= (1 << leds[i]);
            }
            prev_i = i;
            i = (i+direction+4) % 4;
            }
        }
    }