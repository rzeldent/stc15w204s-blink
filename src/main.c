// main.c

// #include <reg51.h>       // old header from SDCC
// #include "STC8Fxx.h"     // Official header from STC-ISP for STC8Fxx / STC8Axx
// #include "STC12C5Axx.h"  // Official header from STC-ISP for STC10Fxx / STC11Fxx / STC12C5Axx / STC12C52xx
// #include "STC12C20xx.h"  // Official header from STC-ISP for STC12C20xx
// #include "STC12C54xx.h"  // Official header from STC-ISP for STC12C54xx
// #include "STC12C56xx.h"  // Official header from STC-ISP for STC12C56xx
#include <STC15Fxx.h>    // Official header from STC-ISP for STC15Wxx / STC15Fxx
// #include "STC15F104E.h"  // Official header from STC-ISP for STC15F204EA / STC15F104E
// #include "STC89xx.h"     // Official header from STC-ISP for STC89xx / STC90xx
// #include "STC90C5xAD.h"  // Official header from STC-ISP for STC90C5xAD

#include <uart.h>
#include <delay.h>
#include <i2c.h>
#include <ssd1306.h>

// Red led
#define LED P33 // P3_3
// Switch to Ground
#define SWITCH P32

unsigned long x = 0;

void main()
{
    //double x = 3.1415;
    uart_init(9600);

    i2c_init();
    ssd1306_init();

    do
    {
        //ssd1306_display_char('R');
        //        uart_send_byte('x');
        //uart_send_string("XY");

        //ssd1306_display_char('5');

        //ssd1306_display_string("Hallo: ");
        ssd1306_display_decimal_byte(x, 0);

        if (SWITCH)
        {
            LED = 1;
            delay_ms(50);
            LED = 0;
            delay_ms(950);
            //ssd1306_set_pos(0, 0);
        }
        else
        {
            ssd1306_clear();
            ssd1306_draw_circle(SSD1306_WIDTH / 2, SSD1306_HEIGHT / 2,  30, 1);
            delay_ms(1000);
        }

        x++;

        //ssd1306_clear();
    } while (1);
}
