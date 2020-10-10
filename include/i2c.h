#pragma once

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

extern void i2c_read_bytes(unsigned char addr, unsigned char reg, unsigned char reg_size, unsigned char *values, unsigned char values_size);