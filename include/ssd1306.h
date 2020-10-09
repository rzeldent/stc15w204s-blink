#pragma once

#define SSD1306_ADDRESS 0x3C     // 011110+SA0+RW - 0x3C or 0x3D
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64


#include <font8x5.h>

extern void ssd1306_init();
extern void ssd1306_clear();
extern void ssd1306_display(unsigned char *image);
extern void ssd1306_set_pos(unsigned char x, unsigned char row);
extern void ssd1306_set_pixel(unsigned char x, unsigned char y, unsigned char color);
extern void ssd1306_draw_circle(unsigned char x, unsigned char y, unsigned char radius, unsigned char color);
extern void ssd1306_display_char(char c);
extern void ssd1306_display_string(char *str);
extern void ssd1306_display_decimal_byte(unsigned long num, unsigned char leading_zeros);
