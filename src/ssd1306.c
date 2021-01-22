#include <i2c.h>
#include <ssd1306.h>
#include <font8x5.h>

#include <uart.h>

// From: https://exploreembedded.com/wiki/OLED_Interface_With_8051

// First byte after start
#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0xC0
#define SSD1306_DATA_CONTINUE 0x40
#define SSD1306_MULTIPLE_COMMANDS 0x80

////////////////////////////////////////////////////////////////////////
// Fundamental Command
#define SSD1306_SETCONTRAST 0x81         // 0x81 + 0-0xFF Contrast ... reset = 0x7F
#define SSD1306_DISPLAYALLON_RESUME 0xA4 // Resume to RAM content display
#define SSD1306_DISPLAYALLON 0xA5        // Resume Ignore RAM content (unused)
#define SSD1306_DISPLAYNORMAL 0xA6       // Normal: 1=white 0=black
#define SSD1306_DISPLAYINVERT 0xA7       // Inverse" 0=white  1=black
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

////////////////////////////////////////////////////////////////////////
// Address Setting Command Table
// 00-0F - set lower nibble of page address
// 10-1F - set upper niddle of page address
#define SSD1306_MEMORYMODE 0x20 // 0x20 + 00 = horizontal, 01 = vertical 2 = page, 3 = illegal
#define SSD1306_MEMORYMODE_HORIZONTAL 0x00
#define SSD1306_MEMORYMODE_VERTICAL 0x01
#define SSD1306_MEMORYMODE_PAGE 0x02
#define SSD1306_SETCOLUMNADDR 0x21 // Only used for horizonal and vertical address modes/ 2 byte Parameter, 0-127 column start address, 0-127 column end address
#define SSD1306_SETPAGEADDR 0x22   // 2 byte parameter, 0-7 page start address, 0-7 page end Address
// 0xB0 -0xB7 ..... Pick page 0-7
#define SSD1306_PAGE_0 0xB0
#define SSD1306_PAGE_1 0xB1
#define SSD1306_PAGE_2 0xB2
#define SSD1306_PAGE_3 0xB3
#define SSD1306_PAGE_4 0xB4
#define SSD1306_PAGE_5 0xB5
#define SSD1306_PAGE_6 0xB6
#define SSD1306_PAGE_7 0xB7

////////////////////////////////////////////////////////////////////////
// Hardware Configuration

#define SSD1306_SETSTARTLINE_ZERO 0x40 // 40-7F - set address startline from 0-127 (6-bits)
// Y Direction
#define SSD1306_SEGREMAPNORMAL 0xA0
#define SSD1306_SEGREMAPINV 0xA1
#define SSD1306_SETMULTIPLEX 0xA8 // 0xA8, number of rows -1 ... e.g. 0xA8, 63
// X Direction
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETDISPLAYOFFSET 0xD3 // double byte with image wrap. Probably should be 0
#define SSD1306_SETCOMPINS 0xDA       // Double Byte Hardware com pins configuration, legal values 0x02, 0x12, 0x022, 0x032

////////////////////////////////////////////////////////////////////////
// Timing and Driving Scheme Settings
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5 // Set display oscillator frequency 0-0xF / clock divide ratio 0-0xF (Default=0x80)
#define SSD1306_SETPRECHARGE 0xD9       // Set pre-changed period (Default = 0x22)
#define SSD1306_SETVCOMDETECT 0xDB      // Set VCOMH Deselected level (Default=0x20)
#define SSD1306_NOP 0xE3

////////////////////////////////////////////////////////////////////////
// Charge Pump Regulator
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_CHARGEPUMP_ON 0x14
#define SSD1306_CHARGEPUMP_OFF 0x10

#define SSD1306_SETLOWCOLUMN 0x01
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

// Scrolling commands
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3

//  Variables
uint8_t ssd1306_page;
int8_t ssd1306_column;

void ssd1306_start_command(uint8_t cmd)
{
    i2c_start(SSD1306_ADDRESS << 1);
    i2c_send(SSD1306_COMMAND);
    i2c_send(cmd);
}

void ssd1306_send_command(uint8_t cmd)
{
    ssd1306_start_command(cmd);
    i2c_stop();
}

void ssd1306_init()
{
    //uart_send_string("ssd1306_init");

    const uint8_t commands[] = {
        // Fundamental Commands
        SSD1306_DISPLAYOFF,
        SSD1306_SETCONTRAST, 0x7F,
        SSD1306_DISPLAYNORMAL,
        // Scrolling Commands
        SSD1306_DEACTIVATE_SCROLL,
        // Addressing Commands
        SSD1306_MEMORYMODE, SSD1306_MEMORYMODE_HORIZONTAL,
        SSD1306_SETSTARTLINE_ZERO | 0x00,
        SSD1306_SETDISPLAYOFFSET, 0x00,
        // Hardware Configuration Commands
        SSD1306_SEGREMAPINV,
        SSD1306_SETMULTIPLEX, SSD1306_HEIGHT - 1,
        SSD1306_COMSCANDEC,
        SSD1306_SETDISPLAYOFFSET, 0x00,
        SSD1306_SETCOMPINS, 0x12,
        // Timing and Driving Settings
        SSD1306_SETDISPLAYCLOCKDIV, 0x80, // Default
        SSD1306_SETPRECHARGE, 0x22,       // Default
        SSD1306_SETVCOMDETECT, 0x20,      // Default
        // Charge pump regulator
        SSD1306_CHARGEPUMP, SSD1306_CHARGEPUMP_ON,
        // Turn the screen back on
        SSD1306_DISPLAYALLON_RESUME,
        SSD1306_DISPLAYON};

    uint16_t i;
    for (i = 0; i < sizeof(commands); ++i)
        ssd1306_send_command(commands[i]);

    ssd1306_clear();
}

void ssd1306_fill_screen(uint8_t b)
{
    //uart_send_string("ssd1306_fill_screen\n");

    uint16_t i;
    ssd1306_set_pos(0, 0);
    for (i = 0; i < SSD1306_WIDTH * SSD1306_HEIGHT / 8; ++i)
        i2c_send(b);
}

void ssd1306_clear()
{
    //uart_send_string("ssd1306_clear\n");

    // Fill with 0
    ssd1306_fill_screen(0);
}

void ssd1306_display(uint8_t *image)
{
    //uart_send_string("ssd1306_display\n");

    unsigned int i;
    ssd1306_set_pos(0, 0);
    // Clear display
    for (i = 0; i < SSD1306_WIDTH * SSD1306_HEIGHT / 8; ++i)
        i2c_send(*image++);

    ssd1306_set_pos(0, 0);
}

void ssd1306_set_pos(uint8_t column, uint8_t page)
{
    //uart_send_string("ssd1306_set_pos\n");

    ssd1306_send_command(SSD1306_SETCOLUMNADDR);
    ssd1306_send_command(column);
    ssd1306_send_command(SSD1306_WIDTH - 1);

    ssd1306_send_command(SSD1306_SETPAGEADDR);
    ssd1306_send_command(page);
    ssd1306_send_command((SSD1306_HEIGHT / 8) - 1);

    ssd1306_page = page;
    ssd1306_column = column;

    i2c_start(SSD1306_ADDRESS << 1);
    i2c_send(SSD1306_DATA_CONTINUE);
}

void ssd1306_set_xy(uint8_t x, uint8_t y)
{
    //uart_send_string("ssd1306_set_xy\n");

    ssd1306_send_command(SSD1306_PAGE_0 + y);
    i2c_send((x >> 4) | SSD1306_SETHIGHCOLUMN);  // | 0x10
    i2c_send((x & 0x0F) | SSD1306_SETLOWCOLUMN); // | 0x01
    i2c_stop();

    i2c_start(SSD1306_ADDRESS << 1);
    i2c_send(SSD1306_DATA_CONTINUE);
}

void ssd1306_set_pixel(uint8_t x, uint8_t y, uint8_t color)
{
    //uart_send_string("ssd1306_set_pixel\n");

    uint8_t mask = 1 << y % 8;
    ssd1306_set_pos(x, y / 8);
    i2c_send(color ? mask : ~mask);
}

void ssd1306_goto_row(uint8_t page)
{
    //uart_send_string("ssd1306_goto_row\n");

    ssd1306_set_pos(page, 0);
}

void ssd1306_go_to_next_row()
{
    //uart_send_string("ssd1306_go_to_next_row\n");

    ssd1306_goto_row(ssd1306_page + 1);
}

void ssd1306_display_char(char c)
{
    //uart_send_string("ssd1306_display_char\n");

    uint8_t i;
    if (c == '\n')
    {
        ssd1306_go_to_next_row();
        return;
    }

    if (ssd1306_column >= SSD1306_WIDTH - FONT_SIZE)
        ssd1306_go_to_next_row();

    // starts from space (0x20)
    c -= ' ';
    for (i = 0; i < FONT_SIZE; ++i)
        i2c_send(font_data[c][i]);

    i2c_send(0x00);
    ssd1306_column += FONT_SIZE + 1;
}

void ssd1306_display_string(char *str)
{
//    uart_send_string("ssd1306_display_string\n");

    while (*str != '\0')
        ssd1306_display_char(*str++);
}

void ssd1306_display_decimal(uint32_t num, uint8_t leading_zeros)
{
    //uart_send_string("ssd1306_display_decimal_byte\n");

    uint32_t devisor = 1000000000;
    uint8_t c;
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

void ssd1306_set_inversion(uint8_t inverse)
{
    ssd1306_send_command(inverse ? SSD1306_DISPLAYINVERT : SSD1306_DISPLAYNORMAL);
}

void ssd1306_flip_horizontal(uint8_t mode)
{
    ssd1306_send_command(mode ? SSD1306_SEGREMAPINV : SSD1306_SEGREMAPNORMAL);
}

void ssd1306_flip_vertical(uint8_t mode)
{
    ssd1306_send_command(mode ? SSD1306_COMSCANINC : SSD1306_COMSCANDEC);
}

void ssd1306_set_brightness(uint8_t brightness)
{
    ssd1306_start_command(SSD1306_SETCONTRAST);
    i2c_send(brightness);
    i2c_stop();
}
