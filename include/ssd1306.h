#pragma once

#include <stdint.h>

#define SSD1306_ADDRESS 0x3C     // 011110+SA0+RW - 0x3C or 0x3D
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#include <font8x5.h>

extern void ssd1306_init();
extern void ssd1306_set_pos(uint8_t row, uint8_t column);
extern void ssd1306_clear();
extern void ssd1306_image(uint8_t *image);
//extern void ssd1306_set_pixel(uint8_t x, uint8_t y, uint8_t color);
extern void ssd1306_display_char(char c);
extern void ssd1306_display_string(char *str);
extern void ssd1306_display_decimal(uint32_t num, uint8_t leading_zeros);
