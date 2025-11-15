#ifndef STM32F407DISCOVERY_DRIVERS_H_
#define STM32F407DISCOVERY_DRIVERS_H_

//RCC
#define RCC_BASE_ADD    (0x40023800)			//Base RCC Address
#define RCC_AHB1ENR     (RCC_BASE_ADD + 0x30)   //Clock for GPIOA
#define RCC_APB2ENR     (RCC_BASE_ADD + 0x44)   //Clock for ADC1

//GPIO's
#define GPIOA_BASE_ADD  (0x40020000)		//GPIOA Base Address
#define GPIOB_BASE_ADD  (0x40020400)		//GPIOB Base Address
#define GPIOD_BASE_ADD  (0x40020C00)		//GPIOD Base Address

#define GPIOA_MODER     (GPIOA_BASE_ADD + 0x00) //GPIOA Port Mode Address
#define GPIOB_MODER		(GPIOB_BASE_ADD + 0x00) //GPIOB Port Mode Address
#define GPIOD_MODER		(GPIOD_BASE_ADD + 0x00) //GPIOD Port Mode Address

#define GPIOB_PORT_OUT_DATA_REG (GPIOB_BASE_ADD + 0x14) //GPIOD Port Output Data Register Address
#define GPIOD_PORT_OUT_DATA_REG (GPIOD_BASE_ADD + 0x14) //GPIOD Port Output Data Register Address

#define GPIOD_BSRR (GPIOD_BASE_ADD + 0x18)


//ADC's
#define ADC1_BASE_ADD   (0x40012000)           //ADC1 Base Address
#define ADC1_SR         (ADC1_BASE_ADD + 0x00) //Status Register
#define ADC1_CR1        (ADC1_BASE_ADD + 0x04) //Set Bit Length (12 bits)
#define ADC1_CR2        (ADC1_BASE_ADD + 0x08) //ADC On & SWSTART
#define ADC1_SMPR2      (ADC1_BASE_ADD + 0x10) //Sample time Register (480 cycles for accuracy)
#define ADC1_SQR1       (ADC1_BASE_ADD + 0x2C) //Amount of conversions
#define ADC1_SQR3       (ADC1_BASE_ADD + 0x34) //Conversion sequence
#define ADC1_DR         (ADC1_BASE_ADD + 0x4C) //Data


#endif
