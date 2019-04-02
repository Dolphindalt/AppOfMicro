#include <msp430g2253.h>
#include "i2c.h"

#define SCL BIT2
#define SDA BIT3
#define clear_sda() P1DIR |= SDA; P1OUT &=~(SDA)
#define clear_scl() P1DIR |= SCL; P1OUT &=~(SCL)
#define set_sda() P1DIR &=~SDA; P1OUT |= SDA
#define set_scl() P1DIR &=~SCL; P1OUT |= SCL
#define DELAY 500
#define i2c_delay() __delay_cycles(DELAY)

static char i2c_read_sda();

void i2c_init()
{
    set_sda();
    set_scl();
    P1REN |= SDA;
    P1REN |= SCL;
    P1OUT &=~SDA;
    P1OUT &=~SCL;
}

char i2c_read_sda()
{
    P1DIR &=~(SDA);
    return (P1IN & SDA) != 0;
}

void i2c_send_start()
{
    set_sda();
    i2c_delay();
    clear_sda();
    i2c_delay();
    clear_scl();
}

void i2c_send_end()
{
    clear_sda();
    i2c_delay();
    set_scl();
    i2c_delay();
    set_sda();
}

void i2c_send_bit(const char b)
{
    if(b) {set_sda();}
    else {clear_sda();}
    i2c_delay();
    set_scl();
    i2c_delay();
    clear_scl();
}

char i2c_read_bit()
{
    char b;
    set_sda();
    i2c_delay();
    set_scl();
    b = i2c_read_sda();
    i2c_delay();
    clear_scl();
    return b;
}

void i2c_send_byte(char byte)
{
    char ctr = 0;
    while(ctr < 8)
    {
        i2c_send_bit((byte & 0x80) != 0);
        byte <<= 1;
        ctr++;
    }
    (void) i2c_read_bit();
}

char i2c_read_byte()
{
    char read, ctr = 0;
    while(ctr < 8)
    {
        read <<= 1;
        read |= i2c_read_bit();
        ctr++;
    }
    return read;
}
