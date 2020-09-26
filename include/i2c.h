#pragma once

#include <8051.h>

#define SDA P0_0
#define SCL P0_1

#define I2C_NAK 1
#define I2C_ACK 0

// Should be done once
extern void i2c_init();

// Send start condition
extern unsigned char i2c_start(unsigned char addr);
// Send stop condition
extern void i2c_stop();
extern unsigned char i2c_send(unsigned char data);
extern unsigned char i2c_read(unsigned char acknac);