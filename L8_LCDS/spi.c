#include "spi.h"
#include <msp430.h>

#define RS BIT2
#define MOSI BIT3
#define CLK BIT4
#define CS BIT5

#define DELAY 250
#define spi_delay() __delay_cycles(DELAY)

void spi_init()
{
    P2DIR |= RS | MOSI | CLK | CS;
    P2OUT |= CS;
    P2OUT &=~(RS | MOSI | CLK);
}

void spi_write(char data, char rw)
{
    char ctr = 0;
    if(rw) P2OUT |= RS;
    else P2OUT &=~RS;
    P2OUT &=~CS;
    while(ctr < 8)
    {
        if((data & 0x80) == 0x80) P2OUT |= MOSI;
        else P2OUT &=~(MOSI);
        data <<= 1;
        P2OUT &=~CLK;
        P2OUT |= CLK;
        spi_delay();
        P2OUT &=~CLK;
        spi_delay();
        ctr++;
    }
    P2OUT |= CS;
    spi_delay();
}
