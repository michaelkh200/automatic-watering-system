#include "moisture_sensor.h"
#include "stm32f407Discovery_Drivers.h"
#include <stdint.h>
#include <stdio.h>



static const uint16_t DRY_VALUES = 2760;
static const uint16_t WET_VALUES = 890;


void moisture_sensor_init(void){

	//Enable Clocks
	uint32_t *pAHB1ENR = (uint32_t*)RCC_AHB1ENR;     //Address of AHB1 Enable Register
	uint32_t *pAPB2ENR = (uint32_t*)RCC_APB2ENR; 	 //Address of APB2 Enable Register
	*pAHB1ENR |= (1 << 0);    //Enable the AHB1 -> GPIOA Clock
	*pAPB2ENR |= (1 << 8);	 //Enable the APB2 -> ADC1 Clock

	//Set GPIO Mode
	uint32_t *pGPIOAModer = (uint32_t*)GPIOA_MODER;    //Address of GPIOA Moder Register
	*pGPIOAModer |= (3 << 0);

	//Configure ADC's
	uint32_t *pADC1Cr1=(uint32_t*)ADC1_CR1;
	uint32_t *pADC1Cr2=(uint32_t*)ADC1_CR2;
	uint32_t *pADC1Sqr1=(uint32_t*)ADC1_SQR1;
	uint32_t *pADC1Smpr2=(uint32_t*)ADC1_SMPR2;

	*pADC1Cr1   &= ~(3 << 24);    // Set 12 bit
	*pADC1Sqr1 &= ~(0xF << 20);   // sequence length = 1 conversion
	*pADC1Smpr2 |= (7 << 0);      // Set 480 cycles for PA0


	*pADC1Cr2 |= (1 << 0);     // ADC on

	}

void pump_and_led_init(void){
	uint32_t *pAHB1ENR = (uint32_t*)RCC_AHB1ENR;     //Address of AHB1 Enable Register
	uint32_t *pGPIOBModer = (uint32_t*)GPIOB_MODER;    //Address of GPIOB Moder Register
	uint32_t *pGPIODModer = (uint32_t*)GPIOD_MODER;    //Address of GPIOD Moder Register

	*pAHB1ENR |= (1 << 1);    //Enable the AHB1 -> GPIOB Clock
	*pAHB1ENR |= (1 << 3);    //Enable the AHB1 -> GPIOD Clock

	*pGPIOBModer |= (1 << 0);  // PB0 Output mode
	*pGPIODModer |= (1 << 24); // Green Output mode
	*pGPIODModer |= (1 << 28); // Red Output mode
}



uint16_t read_moisture_sensor(uint8_t sensor_id){

	uint32_t *pADC1Cr2 =   (uint32_t*)ADC1_CR2;
	uint32_t *pADC1Sqr3 =  (uint32_t*)ADC1_SQR3;
	uint32_t *pADC1Sr =    (uint32_t*)ADC1_SR;
	uint32_t *pADC1Dr =    (uint32_t*)ADC1_DR;

	*pADC1Sqr3 &= ~(0x1F << 0);      //Clear SQ1
	*pADC1Sqr3 |=  (sensor_id << 0); //Set Sq1 = channel 0

	*pADC1Cr2 |= (1 << 30);           //Start ADC Conversion

	while(!((*pADC1Sr >> 1) & 1)){	  //Wait until conversion is finished (EOC == "1")
		//Wait until EOC is "1"
	}

	uint16_t data = (*pADC1Dr & 0xFFF);  // Fetch Data and Clear EOC

	return data;                //Return the data is DR

}


// ADC to 0-100 Conversion
uint8_t convert_to_percentage(uint16_t adc_value){
	uint16_t dry = DRY_VALUES; //Dry (2760)
	uint16_t wet = WET_VALUES; //Wet (890)

	//Conversion of ADC to 0-100
	uint8_t moisture = 100 - (100 * (adc_value - wet) ) / (dry - wet);
	if (moisture < 0) moisture = 0;
	if (moisture > 100) moisture = 100;

	return moisture;
}

//Define Pump State ON or OFF
uint8_t check_moisture_conditions(uint8_t moisture_precent){

	static uint8_t pump_state = 0;
	//DRY
	if (moisture_precent <= 30) {
		pump_state = 1;
		return 1;
	}
	//WET
	else if (moisture_precent >= 80) {
		pump_state = 0;
		return 0;
	}
	//Else previous state!
	return pump_state;

}



//Turn Pump ON or OFF
void pump_on_off(uint8_t moisture_status){

	uint32_t *pGPIOBPortOutReg = (uint32_t*) GPIOB_PORT_OUT_DATA_REG ;
	uint32_t *pGPIOD_BSRR = (uint32_t*) GPIOD_BSRR;

	if(moisture_status == 1){
		//PB0 "1"
		*pGPIOBPortOutReg |=  (1 << 0);  // PB0 = "1"
		//Green Led On "Pump is ON" (LD4)
		*pGPIOD_BSRR = (1 << 12) | (1 << 30);



	} else if (moisture_status == 0){
		//PB0 "0"
		*pGPIOBPortOutReg &= ~(1 << 0);  // PB0 = "0"
		//Red Led On "Pump is OFF" (LD5)
		*pGPIOD_BSRR = (1 << 14) | (1 << 28);


	}

}




