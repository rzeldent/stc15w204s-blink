#pragma once

#define SSD1306_ADDRESS 0x3C
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#include <font8x5.h>

extern void ssd1306_init();
extern void ssd1306_clear();
extern void ssd1306_display_char(char c);
extern void ssd1306_display_string(const char *str);
extern void ssd1306_display_binary_byte(unsigned char num);
extern void ssd1306_display_decimal_byte(unsigned char num);
