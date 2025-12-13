#include "tim2.h"

volatile uint8_t g_tim2_1Hz_flag = 0;

void tim2_init_1Hz(){

    RCC->APB1ENR |= (1u << 0); // Enable RCC for APB1

    TIM2->CR1 = 0u; // reset CR1

    // TIM2CLK = 2 * PCLK1 = 84 MHz (APB1 prescaler != 1)
    // We want counter tick = 1 kHz
    // PSC = (TIM2CLK / f_tick) - 1 = (84 MHz / 1 kHz) - 1 = 84000 - 1
    TIM2->PSC = (8400u) - 1u;
    TIM2->ARR = 10000u  - 1u;// TIM2CLK = 2 * PCLK1 = 84 MHz (APB1 prescaler != 1)

    TIM2->EGR = 1u;// UG = 1

    TIM2->SR = 0u; // Reset SR and clear UIF

    TIM2->DIER |= (1u << 0);      // UIE Interrupt Enable

    NVIC_ISER0 |= (1u << 28); //Interrupt Set Enable Register position 28 enable

    TIM2->CR1 |= (1u << 0);       // CEN Couter Enable
}

// Interrupt service routine – name must match startup file
void TIM2_IRQHandler(){

    // check update interrupt flag
    if (TIM2->SR & (1u << 0))
    {
        TIM2->SR &= ~(1u << 0);   // clear UIF
        g_tim2_1Hz_flag = 1;
    }
}
