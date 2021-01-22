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

void i2c_read_bytes(uint8_t addr, uint8_t reg, uint8_t reg_size, uint8_t *values, uint8_t values_size)
{
	uint8_t i;
	i2c_start(addr);
	// the maximum size of internal address is 3 bytes
	reg_size &= 3;
	// write internal register address - most significant byte first
	while (reg_size-- > 0)
		i2c_send((uint8_t)(reg >> (reg_size * 8)));
	i2c_stop();
	// Read values
	i2c_start(addr);
	for (i = 0; i < values_size; ++i)
		*values++ = i2c_read(i == values_size ? I2C_NAK : I2C_ACK);
	i2c_stop();
}

/*****************************************
 * Write to slave device with
 * slave address e.g. say 0x20
 *****************************************/
/* Init i2c ports first */
//	i2c_init();
/* Send start condition */
//	i2c_start();
/* Send slave address */
//	ack = i2c_send(0x20);
/*
	 * ack == 1 => NAK
	 * ack == 0 => ACK
	 */
//	ack = i2c_send(0x07);
/* Send another data */
//	ack = i2c_send(0x10);
/* Send stop condition */
//	i2c_stop();

/*****************************************
 * Read from slave device with
 * slave address e.g. say 0x20
 *****************************************/
/* Init i2c ports first - Should be done once in main */
//	i2c_init();
/* Send start condition */
//	i2c_start();
/*
	 * Send slave address with Read bit set
	 * So address is 0x20 | 1 = 0x21
	 */
//	i2c_send(0x21);
//	data = i2c_read();
/* Send ack */
//	i2c_ack();
/* Read last byte */
//	data = i2c_read();
/*
	 * Send nak for last byte to indicate
	 * End of transmission
	 */
//	i2c_nak();
/* Send stop condition */
//	i2c_stop();