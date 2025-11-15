#ifndef MOISTURE_SENSOR_H_
#define MOISTURE_SENSOR_H_

#include <stdint.h>
#define SENSOR_0_PA0 (0)



void moisture_sensor_init(void);
void pump_and_led_init(void);
uint16_t read_moisture_sensor(uint8_t sensor_id);
uint8_t convert_to_percentage(uint16_t adc_value);
uint8_t check_moisture_conditions(uint8_t moisture_precent);
void pump_on_off(uint8_t moisture_status);


#endif
