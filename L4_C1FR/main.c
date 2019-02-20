#include <msp430.h>
#include "i2c.h"
#include "led.h"
#include "keypad.h"

int main(void)
 {
    PM5CTL0 &=(~LOCKLPM5);
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	led_init();
	i2c_init();
	keypad_init();

	char s[5] = {'0', '0', '0', '0', '\0' };
	char ctr = 0;

	while(1)
	{
	    keypad_poll();
	    if(keypad_read_char(&s[ctr]))
	    {
	        ctr++;
	        if(ctr == 4) ctr = 0;
	        led_send_chars(0, 0x77, s);
	    }
	}
}
