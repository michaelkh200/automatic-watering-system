#include "moisture_sensor.h"
#include "stm32f407Discovery_Drivers.h"
#include <stdint.h>
#include <stdio.h>



static const uint16_t DRY_VALUES = 2760;
static const uint16_t WET_VALUES = 890;


void moisture_sensor_init(void){
	printf("initializing ADC sensor!\n");

	RCC->AHB1ENR |= (1u << 0u); //Enable RCC for GPIOA
	RCC->APB2ENR |= (1u << 8u); //Enable RCC for ADC1

	//Set GPIOA PA0 as analog mode
	GPIOA->MODER |= (3u << 0u);

	ADC1->CR1 &= ~(3 << 24);		//Set 12 bit
	ADC1->SQR1 &= ~(0xF << 20);		//sequence length = 1 conversion
	ADC1->SMPR1 |= (7 << 0); 		//set 480 cycles for PA0

	ADC1->CR2 |= (1 << 0);     // ADC on

	}

void pump_and_led_init(void){

	RCC->AHB1ENR |= ((1 << 1) | (1 << 3)); //Enable RCC for GPIOB & GPIOD
	GPIOB->MODER |= (1 << 0);		    	//PB0 Output Mode
	GPIOD->MODER |= ((1 << 24) | (1 << 28)); //Red & Green Led output mode
}



uint16_t read_moisture_sensor(uint8_t sensor_id){

	ADC1->SQR3 &= ~(0x1F << 0);      //Clear SQ1
	ADC1->SQR3 |=  (sensor_id << 0); //Set Sq1 = channel 0

	ADC1->CR2 |= (1 << 30);           //Start ADC Conversion

	while(!((ADC1->SR >> 1) & 1)){	  //Wait until conversion is finished (EOC == "1")
		//Wait until EOC is "1"
	}

	uint16_t data = (ADC1->DR & 0xFFF);  // Fetch Data and Clear EOC

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

	if(moisture_status == 1){
		//PB0 "1"
		GPIOB->ODR |=  (1 << 0);  // PB0 = "1"
		//Green Led On "Pump is ON" (LD4)
		GPIOD->BSRR = (1 << 12) | (1 << 30);



	} else if (moisture_status == 0){
		//PB0 "0"
		GPIOB->ODR &= ~(1 << 0);  // PB0 = "0"
		//Red Led On "Pump is OFF" (LD5)
		GPIOD->BSRR = (1 << 14) | (1 << 28);


	}

}




