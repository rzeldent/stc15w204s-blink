// main.c

#include <8052.h>


#define MAIN_Fosc 11059200L

// 定义 LED 驱动管脚
#define LED P3_3

void delay_ms(unsigned int ms)
{
    unsigned int i;
    do
    {
        i = MAIN_Fosc / 13000;
        while (--i)
            ; //14T per loop
    } while (--ms);
}

void main()
{
    while (1)
    {
        LED = 1;
        delay_ms(50);
        LED = 0;
        delay_ms(950);
    }
}
