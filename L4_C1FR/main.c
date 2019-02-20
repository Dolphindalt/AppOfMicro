#include <msp430.h>
#include "i2c.h"
#include "led.h"

int main(void)
 {
    PM5CTL0 &=(~LOCKLPM5);
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	i2c_init();

	char *s = "1234";

	send_char_to_digit(0, 0x77, s);
}
