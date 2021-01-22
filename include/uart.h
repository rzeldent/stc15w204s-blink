#pragma once

#include <stdint.h>

extern void uart_init(uint32_t baudrate);
extern void uart_send_byte(uint8_t data);
extern void uart_send_string(char* str);