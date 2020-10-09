// main.c

#include <stc/STC15Fxx.h>

#include <uart.h>
#include <delay.h>
#include <i2c.h>
#include <ssd1306.h>

// Red led
//#define LED P33 // STC15W204S
#define LED P10 // STC15W408AS
// Switch to Ground
#define SWITCH P32

unsigned char x = 0;

void main()
{
    double x = 3.1415;
    uart_init(9600);

    i2c_init();
    ssd1306_init();

    while (1)
    {
        //ssd1306_display_char('R');
        //        uart_send_byte('x');
        uart_send_string("XY");

        //ssd1306_display_char('5');

        ssd1306_display_string("Hallo!");
        ssd1306_display_decimal_byte(x, 0);

        if (SWITCH)
        {
            LED = 1;
            delay_ms(50);
            LED = 0;
            delay_ms(950);
        }
        else
        {
            ssd1306_clear();
        }

        x++;

        //ssd1306_set_cursor(0, 0);
        //ssd1306_clear();
    }
}
