#include <msp430.h>
#include "spi.h"
#include "lcd.h"
#include "adc.h"

int primitive_to_buffer(char *buffer, unsigned int buffer_size, int primitive);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	__bis_SR_register(GIE);
	
	spi_init();
	lcd_init();
	adc_init();

	char buf[12], i;
	while(1)
	{
	    adc_start_sample();
	    primitive_to_buffer(buf, 12, adc_get_result());
	    lcd_set_ddram(0x00);
	    for(i = 0; buf[i] != '\0'; i++)
	        lcd_write_character(buf[i]);
	    for(; i < 16; i++)
	        lcd_write_character(0x01);
	    __delay_cycles(50000);
	}
}

int primitive_to_buffer(char *buffer, unsigned int buffer_size, int primitive)
{
    if(buffer_size < 2) return 0;
    if(primitive == 0)                          // test special case
    {
            buffer[0] = '0';
            buffer[1] = '\0';
            return 1;
    }
    unsigned int buffer_ptr = 0, i;
    int negative = primitive < 0;
    unsigned int n = negative ? -primitive : primitive;
    while(n)                                    // calculate each digit
    {
        if(buffer_ptr >= buffer_size) return 0;
        buffer[buffer_ptr++] = n % 10 + '0';
        n /= 10;
    }
    if(negative)                                // include the minus if negative
    {
        if(buffer_ptr >= buffer_size) return 0;
        buffer[buffer_ptr++] = '-';
    }
    if(buffer_ptr >= buffer_size) return 0;
    buffer[buffer_ptr] = '\0';
    for(i = 0; i < buffer_ptr/2; i++)           // swap the order of the string
    {
        buffer[i] ^= buffer[buffer_ptr-i-1];
        buffer[buffer_ptr-i-1] ^= buffer[i];
        buffer[i] ^= buffer[buffer_ptr-i-1];
    }
    return 1;
}
