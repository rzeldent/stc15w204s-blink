#pragma once

#include <stdint.h>

#define I2C_NAK 1
#define I2C_ACK 0

// Should be done once
extern void i2c_init();

// Send start condition
extern uint8_t i2c_start(uint8_t addr);
// Send stop condition
extern void i2c_stop();
extern uint8_t i2c_send(uint8_t data);
extern uint8_t i2c_read(uint8_t acknac);

extern void i2c_read_register(uint8_t addr, uint32_t reg, uint8_t reg_size, uint8_t *values, uint16_t values_size);
extern void i2c_send_register(uint8_t addr, uint32_t reg, uint8_t reg_size, uint8_t *values, uint16_t values_size);