#include <stdio.h>
#include <stdint.h>
#include "stm32f407Discovery_Drivers.h"


void init_i2c1(void){

	//Enable RCC for GPIOB & I2C1
	RCC->AHB1ENR |= (1u << 1u);
	RCC->APB1ENR |= (1u << 21u);

	//Set as alternate function
	GPIOB->MODER &= ~((3u << 12u) | (3u << 14u));
	GPIOB->MODER |= ((2u << 12u) | (2u << 14u));

	//Set Open-Drain
	GPIOB->OTYPER |= ((1u << 6u) | (1u << 7u));

	//Set Slew Rate to HIGH
	GPIOB->OSPEEDR &= ~((3u << 12u) | (3u << 14u));
	GPIOB->OSPEEDR |=  ((2u << 12u) | (2u << 14u));

	//clear Pull Up and Pull Down Resistors
	GPIOB->PUPDR &= ~((3u << 12u) | (3u << 14u));

	GPIOB->AFRL &= ~((0xFu << 24u) | (0xFu << 28u));
	GPIOB->AFRL |= ((4u << 24u) | (4u << 28u));

	//Clear register
	I2C1->CR1 = 0;

	//Inform the I2C that were on a 42[MHz] Bus/Frequency
	I2C1->CR2 =42u;

	//Set Standard Mode for I2C1
	//F(CCR) = F(PCLK1) / (2 * F(SCL)) = 42*10^6 / (2 * 100*10^3) = 210
	I2C1->CCR = 210u;

	//T_rise = 1000ns / 23.8ns + 1 = 42 + 1 = 43
	I2C1->TRISE = 43u;

	//Enable I2C
	I2C1->CR1 |= (1u << 0);


}


void i2c1_write(uint32_t len,const uint8_t *data){

	I2C1->CR1 |= (1u << 8);         // start conversion
	while(!((I2C1->SR1) & (0x01))); //SB flag

	/*SEND ADDRESS*/
	I2C1->DR = (SSD1306_ADDR << 1u) | 0;  // Send Led Displays Address
	while(!((I2C1->SR1) & (1u << 1u)));	  // Check if address was sent successfully
	//clear ADDR flag manually
	I2C1->SR1;
	I2C1->SR2;

	/*SEND DATA*/
	for(uint8_t i = 0; i < len; i++){
		while(!((I2C1->SR1) & (1u << 7u))); //TxE
		I2C1->DR = data[i];
	}

	/*STOP TRANSMISSION*/
		//Check BTF
		while(!((I2C1->SR1) & (1u << 2))); //BTF
		//STOP
		I2C1->CR1 |= (1u << 9u);  //STOP
}

//send a COMMAND to the slave
void ssd1306_cmd(uint8_t cmd){
	uint8_t buf[2];
	buf[0] = 0x00u;     //cmd prefix
	buf[1] = cmd;
	i2c1_write(2u,buf);
}

//send DATA to the slave
void ssd1306_data(const uint8_t *data, uint32_t len){

	uint8_t buf[1 + len];
	buf[0] = 0x40u;      //data prefix
	  for (uint32_t i = 0; i < len; i++)
	        buf[1 + i] = data[i];

	i2c1_write(len + 1, buf);

}

// set cursor coordinates on the display
void ssd1306_setCursor(uint8_t page, uint8_t col){
    ssd1306_cmd(0xB0 | page);            // Set page (0–7)
    ssd1306_cmd(0x00 | (col & 0x0F));    // lower nibble
    ssd1306_cmd(0x10 | (col >> 4));      // upper nibble

}

//Display initialization
void ssd1306_init(void){
	ssd1306_cmd(0xAE);      // display off
	ssd1306_cmd(0xD5); ssd1306_cmd(0x80);
	ssd1306_cmd(0xA8); ssd1306_cmd(0x3F);
	ssd1306_cmd(0xD3); ssd1306_cmd(0x00);
	ssd1306_cmd(0x40);
	ssd1306_cmd(0x8D); ssd1306_cmd(0x14);  // charge pump ON
	ssd1306_cmd(0x20); ssd1306_cmd(0x00);  // horizontal addressing
	ssd1306_cmd(0xA1);
	ssd1306_cmd(0xC8);
	ssd1306_cmd(0xDA); ssd1306_cmd(0x12);
	ssd1306_cmd(0x81); ssd1306_cmd(0x7F);
	ssd1306_cmd(0xD9); ssd1306_cmd(0xF1);
	ssd1306_cmd(0xDB); ssd1306_cmd(0x40);
	ssd1306_cmd(0xA4);
	ssd1306_cmd(0xA6);
	ssd1306_cmd(0xAF);      // display on
}

//Clear the Display
void ssd1306_clear(void)
{
    uint8_t zeros[128];
    for (uint32_t i = 0; i < 128; i++) {
        zeros[i] = 0x00;
    }

    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_cmd(0xB0 | page);      // page address
        ssd1306_cmd(0x00);             // lower column
        ssd1306_cmd(0x10);             // upper column
        ssd1306_data(zeros, 128);      // write 128 zero columns
    }
}
// Draw a 7x8 glyph scaled by 3x (width & height) at (startPage, startCol)
void ssd1306_drawGlyph3x_at(const uint8_t *glyph, uint8_t startPage, uint8_t startCol)
{
    const uint8_t srcWidth  = 7u;   // 7 columns
    const uint8_t srcHeight = 8u;   // we use full 8 bits; if your glyph is 7 high, keep one row 0
    const uint8_t scale     = 3u;   // fixed ×3
    const uint8_t dstWidth  = srcWidth * scale; // 7 * 3 = 21 columns
    const uint8_t pages     = 3u;   // 8 * 3 = 24px tall → 3 pages

    uint8_t pageBuf[pages][dstWidth];

    // Clear buffers
    for (uint8_t p = 0; p < pages; p++) {
        for (uint8_t x = 0; x < dstWidth; x++) {
            pageBuf[p][x] = 0x00u;
        }
    }

    // For each source column
    for (uint8_t col = 0; col < srcWidth; col++) {
        uint8_t srcCol = glyph[col];

        // Build a 24-bit tall scaled column
        // col24 bit N = row N of the scaled glyph (0..23)
        uint32_t col24 = 0u;

        for (uint8_t bit = 0; bit < srcHeight; bit++) {
            if (srcCol & (1u << bit)) {
                uint8_t baseRow = (uint8_t)(bit * scale);
                for (uint8_t s = 0; s < scale; s++) {
                    col24 |= (1u << (baseRow + s));  // vertical ×3
                }
            }
        }

        // Split into 3 pages (8 bits each)
        uint8_t b0 = (uint8_t)( col24        & 0xFFu);   // page startPage
        uint8_t b1 = (uint8_t)((col24 >> 8) & 0xFFu);    // page startPage+1
        uint8_t b2 = (uint8_t)((col24 >> 16) & 0xFFu);   // page startPage+2

        // Horizontal ×3: write this column 3 times side-by-side
        for (uint8_t hs = 0; hs < scale; hs++) {
            uint8_t dstX = (uint8_t)(col * scale + hs);
            pageBuf[0][dstX] = b0;
            pageBuf[1][dstX] = b1;
            pageBuf[2][dstX] = b2;
        }
    }

    // Send to display: 3 pages stacked vertically
    for (uint8_t p = 0; p < pages; p++) {
        ssd1306_setCursor((uint8_t)(startPage + p), startCol);
        ssd1306_data(pageBuf[p], dstWidth);
    }
}

// glyph's for 0,1,2,3,4,5,6,7,8,9,%
const uint8_t font0[7] = {
    0x00, //00000000
	0x7F, //00000000
	0x41, //00000000
	0x41, //00000000
	0x41, //00000000
	0x7F, //00000000
	0x00  //00000000
};

const uint8_t font1[7] = {
    0x00, //00000000
	0x02, //00000010
	0x41, //01000001
	0x7F, //01111111
	0x40, //01000000
	0x00, //00000000
	0x00  //00000000
};

const uint8_t font2[7] = {
    0x00, //00000000
	0x79, //01111001
	0x49, //01001001
	0x49, //01001001
	0x49, //01001001
	0x4F, //01001111
	0x00  //00000000
};

const uint8_t font3[7] = {
    0x00, //00000000
	0x49, //01001001
	0x49, //01001001
	0x49, //01001001
	0x49, //01001001
	0x7F, //01111111
	0x00  //00000000
};

const uint8_t font4[7] = {
    0x00, //00000000
	0x0F, //00001111
	0x08, //00001000
	0x08, //00001000
	0x08, //00001000
	0x7F, //01111111
	0x00  //00000000
};

const uint8_t font5[7] = {
    0x00, //00000000
	0x4F, //01001111
	0x49, //01001001
	0x49, //01001001
	0x49, //01001001
	0x79, //01111001
	0x00  //00000000
};

const uint8_t font6[7] = {
    0x00, //00000000
	0x7F, //01111111
	0x49, //01001001
	0x49, //01001001
	0x49, //01001001
	0x79, //01111001
	0x00  //00000000
};


const uint8_t font7[7] = {
    0x00, //0000,0000
    0x01, //0000,0001
    0x61, //0110,0001
    0x11, //0001,0001
    0x0D, //0000,1101
	0x03, //0000,0011
	0x00  //0000,0000
};

const uint8_t font8[7] = {
    0x00, //00000000
	0x7F, //01111111
	0x49, //01001001
	0x49, //01001001
	0x49, //01001001
	0x7F, //01111111
	0x00  //00000000
};

const uint8_t font9[7] = {
    0x00, //00000000
	0x4F, //01001111
	0x49, //01001001
	0x49, //01001001
	0x49, //01001001
	0x7F, //01111111
	0x00  //00000000
};

const uint8_t fontPer[7] = {
    0x43, //01000011
	0x23, //00100011
	0x10, //00010000
	0x08, //00001000
	0x04, //00000100
	0x62, //01100010
	0x61  //01100001
};

//get the digits of the moisture percentage
uint8_t getThird(uint8_t num){
	return num / 100;
}
uint8_t getSecond(uint8_t num){
	return (num / 10) % 10;
}
uint8_t getFirst(uint8_t num){
	return num % 10;
}

//select the proper glyph based a single digit
const uint8_t* selectGlyph(uint8_t num){
	switch(num){
	case 0 : return font0;
	case 1 : return font1;
	case 2 : return font2;
	case 3 : return font3;
	case 4 : return font4;
	case 5 : return font5;
	case 6 : return font6;
	case 7 : return font7;
	case 8 : return font8;
	case 9 : return font9;
	default: return font0;
	}
}

//Print 3-1 digits + % on the screen
void print_LED(uint8_t moisture_percentage){
	uint8_t third  = getThird(moisture_percentage);
	uint8_t second  = getSecond(moisture_percentage);
	uint8_t first  = getFirst(moisture_percentage);
	if (third == 0 && second == 0){
	    ssd1306_drawGlyph3x_at(selectGlyph(first), 3, 65);
	    ssd1306_drawGlyph3x_at(fontPer, 3, 90);
		} else if(third == 0){
			ssd1306_drawGlyph3x_at(selectGlyph(second), 3, 44);
			ssd1306_drawGlyph3x_at(selectGlyph(first), 3, 65);
			ssd1306_drawGlyph3x_at(fontPer, 3, 90);
			} else {
        		ssd1306_drawGlyph3x_at(selectGlyph(third), 3, 23);
		        ssd1306_drawGlyph3x_at(selectGlyph(second), 3, 44);
				ssd1306_drawGlyph3x_at(selectGlyph(first), 3, 65);
			    ssd1306_drawGlyph3x_at(fontPer, 3, 90);
	}

}



