#include <msp430fr5969.h>
#include "keypad.h"
#include "uart.h"

int main()
{
    PM5CTL0 &=(~LOCKLPM5);
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    uart_init();
    keypad_init();

    while(1)
    {
        char c;
        int code = keypad_read_char(&c);
        if(code)
        {
            uart_write_char(c);
            put_newline();
        }
    }
}
