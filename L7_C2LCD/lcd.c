#include "lcd.h"
#include <msp430.h>
#include "spi.h"

void lcd_init()
{
    P2DIR |= BIT1;
    P2OUT |= BIT1; // turn on backlight
    spi_write(0x30, 0);
    spi_write(0x30, 0);
    spi_write(0x30, 0);
    spi_write(0x39, 0);
    spi_write(0x14, 0);
    spi_write(0x56, 0);
    spi_write(0x6D, 0);
    spi_write(0x70, 0);
    spi_write(0x0C, 0);
    spi_write(0x06, 0);
    spi_write(0x01, 0);
    __delay_cycles(10);
}

void lcd_set_ddram(char address)
{
    address |= 0x80;
    spi_write(address, 0);
}

void lcd_write_character(char c)
{
    spi_write(c, 1);
}
