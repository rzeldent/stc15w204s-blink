// main.c

#include <8052.h>

#include <delay.h>

#include <i2c.h>
#include <ssd1306.h>

// Red led
#define LED P3_3
// Switch to Ground
#define SWITCH P3_2

void main()
{
    unsigned char x = 0;
    i2c_init();

    ssd1306_init();

    while (1)
    {
        ssd1306_display_string("hello: ");
        ssd1306_display_decimal_byte(x);
        if (SWITCH)
        {
            LED = 1;
            delay_ms(50);
            LED = 0;
            delay_ms(950);
        }
        else
        {
            LED = 1;
        }
        x++;
        ssd1306_clear();
    }
}
