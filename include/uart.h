#pragma once

extern void uart_init(unsigned long baudrate);
extern void uart_send_byte(unsigned char data);
extern void uart_send_string(char* str);