#include <STC15Fxx.h>

#include <uart.h>
#include <oscilator.h>

void uart_init(unsigned long baudrate)
{
    SCON = 0x5A; // Set the serial port to 8-bit variable baud rate
    T2L = (65536 - OSCILATOR_FREQ / 4 / baudrate);
    T2H = (65536 - OSCILATOR_FREQ / 4 / baudrate) >> 8;
    AUXR = 0x14;  // T2 in T1 mode, and run T2
    AUXR |= 0x01; // Select T2 as uart 1 baud-rate generator
}

void uart_send_byte(unsigned char data)
{
    while (!TI)
        ;
    TI = 0;
    SBUF = data;
}

void uart_send_string(char *str)
{
    while (*str != '\0')
        uart_send_byte(*str++);
}