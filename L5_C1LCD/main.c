#include <msp430.h>
#include "lcd.h"
#include "spi.h"
#include "keypad.h"

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	__bis_SR_register(GIE);

	keypad_init();
	spi_init();
	lcd_init();

	lcd_set_ddram(0x00);
	char c;
	while(1)
	{
	    keypad_poll();
	    __delay_cycles(50000);
	    if(keypad_read_char(&c))
	    {
	        lcd_write_character(c);
	    }
	}
}
