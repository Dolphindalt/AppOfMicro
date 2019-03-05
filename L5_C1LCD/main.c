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
	lcd_display_on_off(1, 0, 1);

	lcd_set_ddram(0x00);
	char c;
	while(1)
	{
	    keypad_poll();
	    if(keypad_read_char(&c))
	    {
	        if(c == '*')
	            lcd_delete_backward(1);
	        else
	            lcd_write_character(c);
	    }
	}
}
