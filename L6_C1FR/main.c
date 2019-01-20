#include <msp430fr5969.h>
#include "uart.h"

int main(void)
{
    PM5CTL0 &=(~LOCKLPM5);
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	uart_init();

	while(1)
	{
	    char s = uart_read_char();
	    put_newline();
	    uart_write_char(s);
	    put_newline();
	}
}
