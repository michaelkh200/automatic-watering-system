#include <stdint.h>
#include <stdio.h>
#include "stm32f407Discovery_Drivers.h"



void clock_init(){

	RCC->CR |= (1u << 0);			//RCC for HSI
	while(!(RCC->CR & (1u << 1u)));	//HSI Ready flag

	FLASH->ACR =0;
	FLASH->ACR |= (5u << 0);
	FLASH->ACR |= (1u << 8);
	FLASH->ACR |= (1u << 9);
	FLASH->ACR |= (1u << 10);

		const uint32_t PLLM = 16u; //Fvco_in = F(hsi) / PLLM =====> Fvco_in = 1MHz
	    const uint32_t PLLN = 336u;//Fvco_out = Fvco_in * PLLN ===> Fvco_out = 336MHz
	    // Fvco_clock = Fin * (PPLN/PPLM) = 16M * (336/16) 16M * 21 ===> 336MHz
	    const uint32_t PLLP = 2u; //Fppl_general_clock = Fsysclk = Fvco_clock / PPLP ===> 168M / 336M = 1/2;
	    const uint32_t PLLQ = 7u; //48M = 336M / PPLQ

	    RCC->PLLCFGR = 0; // reset PLL Configurations
	    RCC->PLLCFGR |= (PLLM & 0x3F);
	    RCC->PLLCFGR |= ((PLLN & 0x1FF) << 6u);
	    RCC->PLLCFGR |= ((((PLLP / 2u) -1u) & 0x3)<< 16u);
	    RCC->PLLCFGR |= ((PLLQ & 0xF ) << 24u);

	    RCC->CR |= (1u << 24u); // PPLON
	    while(!(RCC->CR & (1u << 25u))); //PPLRDY

	    RCC->CFGR = 0; //Reset Register
	    RCC->CFGR |= (0u << 4u);  // AHB  /0
	    RCC->CFGR |= (5u << 10u); // APB1 /4
	    RCC->CFGR |= (4u << 13u); // APB2 /2

	    RCC->CFGR &= ~(3u << 0); //Clear
	    RCC->CFGR |=  (2u << 0); //Set PPL as SYSTEM CLOCK

	    while(((RCC->CFGR  >> 2u) & 3u) != 2u); // Wait for SWS to update to PPL

}
