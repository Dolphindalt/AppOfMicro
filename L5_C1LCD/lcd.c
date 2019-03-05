#include "lcd.h"
#include <msp430.h>
#include "spi.h"

char current_address = 0x00;

static void increment_address()
{
    current_address++;
    if(current_address == 0x10)
    {
        lcd_set_ddram(0x40);
        current_address = 0x40;
    }
    else if(current_address == 0x50)
    {
        lcd_set_ddram(0x00);
        current_address = 0x00;
    }
}

static void decrement_address()
{
    if(current_address == 0x00)
    {
        current_address = 0x50;
    }
    else if(current_address == 0x40)
    {
        current_address = 0x10;
    }
    current_address--;
    lcd_set_ddram(current_address);
}

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

void lcd_display_on_off(char display_on, char cursor_on, char cursor_blink_on)
{
    char cmd = 0x08;
    if(display_on) cmd |= 0x04;
    if(cursor_on) cmd |= 0x02;
    if(cursor_blink_on) cmd |= 0x01;
    spi_write(cmd, 0);
}

void lcd_set_ddram(char address)
{
    address |= 0x80;
    spi_write(address, 0);
}

void lcd_write_character(char c)
{
    spi_write(c, 1);
    increment_address();
}

void lcd_delete_backward(char offset)
{
    while(offset-- ^ 0)
    {
        decrement_address();
        lcd_write_character(0x00);
        decrement_address();
    }
}
