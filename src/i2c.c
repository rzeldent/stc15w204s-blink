#include <i2c.h>

void i2c_init()
{
	SDA = 1;
	SCL = 1;
}

unsigned char i2c_start(unsigned char addr)
{
	SDA = 0;
	SCL = 0;
	return i2c_send(addr);
}

void i2c_stop()
{
	SCL = 0;
	SDA = 0;
	SCL = 1;
	SDA = 1;
}

unsigned char i2c_send(unsigned char data)
{
	unsigned char i;
	unsigned char ack_bit;
	for (i = 0; i < 8; i++)
	{
		SDA = (data & 0x80) != 0;
		SCL = 1;
		SCL = 0;
		data <<= 1;
	}
	SDA = 1;
	SCL = 1;
	ack_bit = SDA;
	SCL = 0;
	return ack_bit;
}

unsigned char i2c_read(unsigned char acknak)
{
	unsigned char i;
	unsigned char data = 0;
	for (i = 0; i < 8; i++)
	{
		SCL = 1;
		if (SDA)
			data |= 1;
		if (i < 7)
			data <<= 1;
		SCL = 0;
	}
	SDA = acknak;
	SCL = 1;
	SCL = 0;
	SDA = 1;

	return data;
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