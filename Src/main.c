#include "moisture_sensor.h"
#include <stdint.h>
#include <stdio.h>

//
// Needed just to print for the console used more for debugging.
//
#define GPIOB_BASE_ADD  (0x40020400)
#define GPIOB_PORT_OUT_DATA_REG (GPIOB_BASE_ADD + 0x14)
uint32_t *pGPIOBPortOutReg = (uint32_t*) GPIOB_PORT_OUT_DATA_REG;
//
//
//

//ADC & Pump initialization
void system_init(void){
	    moisture_sensor_init();
	    pump_and_led_init();
}

//Delay 1 second (approximately)
void delay(void){
	for (volatile uint32_t i = 0 ; i < 3000000; i++){

	}
}


int main(void){
	//initialize moisture sensor + pump
	system_init();

	printf("Starting conversions!\n\n\n\n\n");

	// 1 second loop that takes the ADC sample and activates/deactivates the pump
	while(1){
	//ADC value
	uint16_t moisture_adc_val1 = read_moisture_sensor(SENSOR_0_PA0);
	//Convert ADC to 0-100
	uint8_t moisture_percentage = convert_to_percentage(moisture_adc_val1);
	//Pump State On / Off
	uint8_t moisture_status = check_moisture_conditions(moisture_percentage);
	//Print Pump State + Percentage
	printf("Moisture Status : %d  Moisture Percent : %d%%\n",moisture_status, moisture_percentage);

	//Turn Pump OFF
	if(moisture_status == 0){
		pump_on_off(moisture_status);

		// Console prints
		printf("off\n");
		printf("RegOutBData : %08lX\n", *pGPIOBPortOutReg);

	}
	//Turn Pump ON
	else if(moisture_status == 1){
		pump_on_off(moisture_status);

		//Console prints
		printf("On\n");
		printf("RegOutBData : %08lX\n", *pGPIOBPortOutReg);


	}

	// 1 Second Delay (Approximately)
	delay();
	}

}
