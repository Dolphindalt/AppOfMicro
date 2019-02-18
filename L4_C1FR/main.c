#include <msp430.h>
#include "i2c.h"
#include "led.h"

int main(void)
 {
    PM5CTL0 &=(~LOCKLPM5);
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	i2c_init();

	char *s = "1";

	while(1)
	{
	    send_char_to_digit(0x01, DISPLAY_FOUR_CONTROL_BITS, s);
	}
}
