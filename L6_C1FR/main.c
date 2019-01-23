#include <msp430fr5969.h>
#include "uart.h"

int main(void)
{
    PM5CTL0 &=(~LOCKLPM5);
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	uart_init();

	while(1)
	{
	    char buffer[50];
	    primitive_to_buffer(buffer, 50, 1);
	    uart_write_string(buffer);
	    put_newline();
	    primitive_to_buffer(buffer, 50, -1);
	    uart_write_string(buffer);
	    put_newline();
	    primitive_to_buffer(buffer, 50, 0);
	    uart_write_string(buffer);
	    put_newline();
	    uart_read_char();
	    put_newline();
	}
}
