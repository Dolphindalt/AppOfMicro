#include <msp430.h>
#include "lcd.h"
#include "spi.h"

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	__bis_SR_register(GIE);

	lcd_init();
	spi_init();

	char s[5] = { 0x03, '\0' };
	while(1)
	{
	    lcd_write_characters(s);
	    __delay_cycles(50000);
	}
}
