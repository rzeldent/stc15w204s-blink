#pragma once

#include <8051.h>

#define SDA P0_0
#define SCL P0_1

extern void i2c_init();
extern void i2c_start();
extern void i2c_restart();
extern void i2c_stop();
extern void i2c_ack();
extern void i2c_nak();
extern unsigned char i2c_send(unsigned char data);
extern unsigned char i2c_read();