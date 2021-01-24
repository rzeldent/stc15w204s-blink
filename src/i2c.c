#include <STC15Fxx.h>

#include <demoboards.h>
#include <i2c.h>

void i2c_init()
{
	I2C_SDA = 1;
	I2C_SCL = 1;
}

uint8_t i2c_start(uint8_t addr)
{
	I2C_SDA = 0;
	I2C_SCL = 0;
	return i2c_send(addr);
}

void i2c_stop()
{
	I2C_SCL = 0;
	I2C_SDA = 0;
	I2C_SCL = 1;
	I2C_SDA = 1;
}

uint8_t i2c_send(uint8_t data)
{
	uint8_t i;
	uint8_t ack_bit;
	for (i = 0; i < 8; i++)
	{
		I2C_SDA = (data & 0x80) != 0;
		I2C_SCL = 1;
		I2C_SCL = 0;
		data <<= 1;
	}
	I2C_SDA = 1;
	I2C_SCL = 1;
	ack_bit = I2C_SDA;
	I2C_SCL = 0;
	return ack_bit;
}

uint8_t i2c_read(uint8_t acknak)
{
	uint8_t i;
	uint8_t data = 0;
	for (i = 0; i < 8; i++)
	{
		I2C_SCL = 1;
		if (I2C_SDA)
			data |= 1;
		if (i < 7)
			data <<= 1;
		I2C_SCL = 0;
	}
	I2C_SDA = acknak;
	I2C_SCL = 1;
	I2C_SCL = 0;
	I2C_SDA = 1;

	return data;
}

void i2c_read_register(uint8_t addr, uint32_t reg, uint8_t reg_size, uint8_t *values, uint16_t values_size)
{
	i2c_start(addr);
	// the maximum size of internal address is 3 bytes
	reg_size &= 3;
	// write internal register address - most significant byte first
	while (reg_size--)
		i2c_send((uint8_t)(reg >> (reg_size * 8)));

	i2c_stop();
	// Read values
	i2c_start(addr);

	if (values_size--)
	{
		while (values_size--)
			*values++ = i2c_read(I2C_ACK);

		*values = i2c_read(I2C_NAK);
	}

	i2c_stop();
}

void i2c_send_register(uint8_t addr, uint32_t reg, uint8_t reg_size, uint8_t *values, uint16_t values_size)
{
	i2c_start(addr);
	// the maximum size of internal address is 3 bytes
	reg_size &= 3;
	// write internal register address - most significant byte first
	while (reg_size--)
		i2c_send((uint8_t)(reg >> (reg_size * 8)));

	while (values_size--)
		i2c_send(*values++);

	i2c_stop();
}