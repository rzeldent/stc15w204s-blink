// main.c

#include <8052.h>

#define oscillator_freq 11059200L

// Red led
#define LED P3_3
// Switch to Ground
#define SWITCH P3_2

void delay_ms(unsigned int ms)
{
    unsigned int i;
    do
    {
        i = oscillator_freq / 13000;
        while (--i)
            ; //14T per loop
    } while (--ms);
}

void main()
{
    while (1)
    {
        if (SWITCH)
        {
            LED = 1;
            delay_ms(50);
            LED = 0;
            delay_ms(950);
        } else
        {
            LED=1;
        }
        
    }
}
