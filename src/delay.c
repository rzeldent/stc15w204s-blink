# include <delay.h>

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
