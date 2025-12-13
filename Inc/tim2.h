
#ifndef TIM2_H_
#define TIM2_H_


#include <stdint.h>
#include "stm32f407Discovery_Drivers.h"

// global 1 Hz tick flag
extern volatile uint8_t g_tim2_1Hz_flag;

// init TIM2 to generate 1 interrupt per second
void tim2_init_1Hz();

#endif
