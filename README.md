# automatic-watering-system
Bare-metal STM32F407 automatic plant watering system using ADC moisture sensing and relay-controlled pump.
Current features:
* Reads soil moisture via ADC1 (PA0)
* Drives a relay-controlled pump based on a moisture threshold
* Shows live moisture percentage on an SSD1306 I²C OLED
