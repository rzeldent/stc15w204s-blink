// main.c

#include <8052.h>

#include <i2c.h>
#include <delay.h>

// Red led
#define LED P3_3
// Switch to Ground
#define SWITCH P3_2

void main()
{
    i2c_init();

    while (1)
    {
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
    }
}
