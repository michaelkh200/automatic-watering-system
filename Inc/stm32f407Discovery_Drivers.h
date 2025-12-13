#ifndef STM32F407DISCOVERY_DRIVERS_H_
#define STM32F407DISCOVERY_DRIVERS_H_

/* BASE ADDRESSES*/
#define RCC_BASE_ADD    (0x40023800u)			//Base RCC Address
//GPIO's
#define GPIOA_BASE_ADD  (0x40020000u)		//GPIOA Base Address
#define GPIOB_BASE_ADD  (0x40020400u)		//GPIOB Base Address
#define GPIOD_BASE_ADD  (0x40020C00u)		//GPIOD Base Address
//I2C
#define I2C1_BASE_ADD (0x40005400u)
////ADC's
#define ADC1_BASE_ADD (0x40012000)           //ADC1 Base Address

//NVIC
#define NVICRBB_BASE_ADD (0xE000E100u)
#define NVIC_ISER0   (*(volatile uint32_t*)(NVICRBB_BASE_ADD + 0x00))

//FLASH
#define FLASH_BASE_ADD   (0x40023C00u)

//TIM
#define TIM2_BASE_ADD (0x40000000u)


//SSD1306 Led Display Address
#define SSD1306_ADDR  (0x3Cu)


// RCC peripheral register definition structure
typedef struct{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR ;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	volatile uint32_t RESERVED1;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t RESERVED2;
	volatile uint32_t RESERVED3;
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t RESERVED4;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t RESERVED5;
	volatile uint32_t RESERVED6;
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	volatile uint32_t RESERVED7;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t RESERVED8;
	volatile uint32_t RESERVED9;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t RESERVED10;
	volatile uint32_t RESERVED11;
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t PLLSAICFGR;
	volatile uint32_t DCKCFGR;

}RCC_regDef_t;

// ADC peripheral register definition structure
typedef struct{
	volatile uint32_t SR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMPR1;
	volatile uint32_t SMPR2;
	volatile uint32_t JOFR1;
	volatile uint32_t JOFR2;
	volatile uint32_t JOFR3;
	volatile uint32_t JOFR4;
	volatile uint32_t HTR;
	volatile uint32_t LTR;
	volatile uint32_t SQR1;
	volatile uint32_t SQR2;
	volatile uint32_t SQR3;
	volatile uint32_t JSQR;
	volatile uint32_t JDR1;
	volatile uint32_t JDR2;
	volatile uint32_t JDR3;
	volatile uint32_t JDR4;
	volatile uint32_t DR;

}ADC_regDef_t;

// I2C peripheral register definition structure
typedef struct {
	volatile uint32_t  CR1;   //0x00
	volatile uint32_t  CR2;   //0x04
	volatile uint32_t  OAR1;  //0x08
	volatile uint32_t  OAR2;  //0x0C
	volatile uint32_t  DR;    //0x10
	volatile uint32_t  SR1;   //0x14
	volatile uint32_t  SR2;   //0x18
	volatile uint32_t  CCR;   //0x1C
	volatile uint32_t  TRISE; //0x20
	volatile uint32_t  FLTR;  //0x24

}I2C_regDef_t;

//GPIOx peripheral register definition structure
typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;

}GPIOx_regDef_t;

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RESERVED0;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t RESERVED1;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIMx_regDef_t;

typedef struct
{
    volatile uint32_t ACR;      // 0x00: Access control register
    volatile uint32_t KEYR;     // 0x04: Key register
    volatile uint32_t OPTKEYR;  // 0x08: Option key register
    volatile uint32_t SR;       // 0x0C: Status register
    volatile uint32_t CR;       // 0x10: Control register
    volatile uint32_t OPTCR;    // 0x14: Option control register
} FLASH_regDef_t;


// Peripheral register definitions
#define RCC ((RCC_regDef_t*)RCC_BASE_ADD)
#define GPIOA ((GPIOx_regDef_t*)GPIOA_BASE_ADD)
#define GPIOB ((GPIOx_regDef_t*)GPIOB_BASE_ADD)
#define GPIOD ((GPIOx_regDef_t*)GPIOD_BASE_ADD)
#define ADC1 ((ADC_regDef_t*)ADC1_BASE_ADD)
#define I2C1 ((I2C_regDef_t*)I2C1_BASE_ADD)
#define FLASH ((FLASH_regDef_t*)FLASH_BASE_ADD)
#define TIM2 ((TIMx_regDef_t*)TIM2_BASE_ADD)


#endif
