#ifndef I2C_H
#define I2C_H

void i2c_init();

void i2c_send_address(char a, char rw);

void i2c_send_start();

void i2c_send_end();

void i2c_send_bit(const char b);

char i2c_read_bit();

void i2c_send_byte(char byte);

char i2c_read_byte();

#endif
