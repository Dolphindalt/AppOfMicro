#include "lcd.h"
#include <msp430.h>
#include "spi.h"

void lcd_init()
{
    P2DIR |= BIT1;
    P2OUT |= BIT1; // turn on backlight
    spi_write(0x30, 0);
    __delay_cycles(1);
    spi_write(0x0F, 0);
    __delay_cycles(1);
}

void lcd_set_ddram(char address)
{
    address |= 0x70;
    spi_write(address, 0);
}

void lcd_write_characters(char *characters)
{
    char ctr = 0;
    while(characters[ctr] != '\0')
    {
        spi_write(characters[ctr], 1);
        ctr++;
    }
}
