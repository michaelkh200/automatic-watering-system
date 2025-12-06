#ifndef LED_DISPLAY_H_
#define LED_DISPLAY_H_

void init_i2c1(void);
void i2c1_write(uint32_t len,const uint8_t *data);
void ssd1306_cmd(uint8_t cmd);
void ssd1306_data(const uint8_t *data, uint32_t len);
void ssd1306_setCursor(uint8_t page, uint8_t col);
void ssd1306_init(void);
void ssd1306_clear(void);
void ssd1306_drawGlyph3x_at(const uint8_t *glyph, uint8_t startPage, uint8_t startCol);

extern const uint8_t font0[7];
extern const uint8_t font1[7];
extern const uint8_t font2[7];
extern const uint8_t font3[7];
extern const uint8_t font4[7];
extern const uint8_t font5[7];
extern const uint8_t font6[7];
extern const uint8_t font7[7];
extern const uint8_t font8[7];
extern const uint8_t font9[7];
extern const uint8_t fontPer[7];

const uint8_t* selectGlyph(uint8_t num);
uint8_t getFirst(uint8_t num);
uint8_t getSecond(uint8_t num);
uint8_t getThird(uint8_t num);
void print_LED(uint8_t moisture_percentage);





#endif /* LED_DISPLAY_H_ */
