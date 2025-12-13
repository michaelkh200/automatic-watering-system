#include <stdint.h>
#include <stdio.h>
#include "stm32f407Discovery_Drivers.h"
#include "clock.h"
#include "led_display.h"
#include "moisture_sensor.h"
#include "tim2.h"


//ADC, Pump, I2C , SSD1306 initialization
void system_init(void){
		clock_init();
	    moisture_sensor_init();
	    pump_and_led_init();
	    init_i2c1();
	    ssd1306_init();
	    tim2_init_1Hz();
}

//volatile uint8_t g_tim2_1Hz_flag;

//Delay 1 second (approximately)
void delay(void){
    while (!g_tim2_1Hz_flag);
    g_tim2_1Hz_flag = 0;
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
	}

	//Turn Pump ON
	else if(moisture_status == 1){
		pump_on_off(moisture_status);
		//Console prints
		printf("On\n");
	}
		//clear the previous content on the led and print the new one
	    ssd1306_clear();
	    print_LED(moisture_percentage);
	// 1 second delay
	delay();
	}

}
