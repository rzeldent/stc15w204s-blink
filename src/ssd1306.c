#include <i2c.h>
#include <ssd1306.h>
#include <font8x5.h>

// From: https://exploreembedded.com/wiki/OLED_Interface_With_8051

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0xC0
#define SSD1306_DATA_CONTINUE 0x40

#define SSD1306_SET_CONTRAST_CONTROL 0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME 0xA4
#define SSD1306_DISPLAY_ALL_ON 0xA5
#define SSD1306_NORMAL_DISPLAY 0xA6
#define SSD1306_INVERT_DISPLAY 0xA7
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF
#define SSD1306_NOP 0xE3

#define SSD1306_HORIZONTAL_SCROLL_RIGHT 0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT 0x27
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT 0x29
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT 0x2A
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3

#define SSD1306_SET_LOWER_COLUMN 0x00
#define SSD1306_SET_HIGHER_COLUMN 0x10
#define SSD1306_MEMORY_ADDR_MODE 0x20
#define SSD1306_SET_COLUMN_ADDR 0x21
#define SSD1306_SET_PAGE_ADDR 0x22

#define SSD1306_SET_START_LINE 0x40
#define SSD1306_SET_SEGMENT_REMAP 0xA0
#define SSD1306_SET_MULTIPLEX_RATIO 0xA8
#define SSD1306_COM_SCAN_DIR_INC 0xC0
#define SSD1306_COM_SCAN_DIR_DEC 0xC8
#define SSD1306_SET_DISPLAY_OFFSET 0xD3
#define SSD1306_SET_COM_PINS 0xDA
#define SSD1306_CHARGE_PUMP 0x8D

#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO 0xD5
#define SSD1306_SET_PRECHARGE_PERIOD 0xD9
#define SSD1306_SET_VCOM_DESELECT 0xDB

//  Variables
unsigned int ssd1306_line;
unsigned int ssd1306_column;

void ssd1306_send_command(unsigned char cmd)
{
    i2c_start(SSD1306_ADDRESS << 1);
    i2c_send(SSD1306_COMMAND);
    i2c_send(cmd);
    i2c_stop();
}

void ssd1306_init()
{
    const unsigned char commands[] = {
        SSD1306_DISPLAY_OFF,
        SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO, 0x80,
        SSD1306_SET_MULTIPLEX_RATIO, 0x3F,
        SSD1306_SET_DISPLAY_OFFSET, 0x0,
        SSD1306_SET_START_LINE | 0x0,
        SSD1306_CHARGE_PUMP, 0x14,
        SSD1306_MEMORY_ADDR_MODE, 0x00,
        SSD1306_SET_SEGMENT_REMAP | 0x1,
        SSD1306_COM_SCAN_DIR_DEC,
        SSD1306_SET_COM_PINS, 0x12,
        SSD1306_SET_CONTRAST_CONTROL, 0xCF,
        SSD1306_SET_PRECHARGE_PERIOD, 0xF1,
        SSD1306_SET_VCOM_DESELECT, 0x40,
        SSD1306_DISPLAY_ALL_ON_RESUME,
        SSD1306_NORMAL_DISPLAY,
        SSD1306_DISPLAY_ON};

    unsigned int i;
    for (i = 0; i < sizeof(commands); ++i)
        ssd1306_send_command(commands[i]);

    ssd1306_clear();
}

void ssd1306_clear()
{
    unsigned int i;

    ssd1306_set_cursor(0, 0);

    i2c_start(SSD1306_ADDRESS << 1);
    i2c_send(SSD1306_DATA_CONTINUE);
    // Clear display
    for (i = 0; i < SSD1306_WIDTH * SSD1306_HEIGHT / 8; ++i)
        i2c_send(0);

    ssd1306_set_cursor(0, 0);
}

void ssd1306_set_cursor(unsigned char line, unsigned char column)
{
    ssd1306_send_command(SSD1306_SET_COLUMN_ADDR);
    ssd1306_send_command(column);
    ssd1306_send_command(127);

    ssd1306_send_command(SSD1306_SET_PAGE_ADDR);
    ssd1306_send_command(line);
    ssd1306_send_command(7);

    i2c_start(SSD1306_ADDRESS << 1);
    i2c_send(SSD1306_DATA_CONTINUE);

    ssd1306_line = line;
    ssd1306_column = column;
}

void ssd1306_goto_line(unsigned char line)
{
    ssd1306_set_cursor(line, 0);
}

void ssd1306_go_to_next_line()
{
    ssd1306_goto_line(ssd1306_line + 1);
}

void ssd1306_display_char(char c)
{
    unsigned char i;

    if (c == '\n')
    {
        ssd1306_go_to_next_line();
        return;
    }

    if (ssd1306_column >= SSD1306_WIDTH - FONT_SIZE)
        ssd1306_go_to_next_line();

    // starts from space (0x20)
    c = c - ' ';
    for (i = 0; i < FONT_SIZE; ++i)
    {
        i2c_send(font_data[c][i]);
        ssd1306_column++;
    }

    i2c_send(0x00);
    ssd1306_column++;
}

void ssd1306_display_string(char *str)
{
    while (*str != '\0')
        ssd1306_display_char(*str++);
}

void ssd1306_display_binary_byte(unsigned char num)
{
    unsigned char i = 7;
    do
    {
        ssd1306_display_char(num & 0x80 ? '1' : '0');
    } while (i--);
}

void ssd1306_display_decimal_byte(unsigned char num, unsigned char leading_zeros)
{
    unsigned char devisor = 100;
    unsigned char c;
    while (devisor > 1)
    {
        c = num / devisor;
        if (leading_zeros || c > 0)
        {
            num -= c * devisor;
            ssd1306_display_char('0' + c);
            leading_zeros = 1;
        }
        devisor /= 10;
    }

    ssd1306_display_char('0' + num);
}

void ssd1306_set_inversion(unsigned char inverse)
{
    ssd1306_send_command(inverse ? SSD1306_INVERT_DISPLAY : SSD1306_NORMAL_DISPLAY);
}

void ssd1306_set_brightness(unsigned char brightness)
{
    ssd1306_send_command(SSD1306_SET_CONTRAST_CONTROL);
    ssd1306_send_command(brightness);
}