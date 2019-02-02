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
        char code = keypad_poll();
        if(code)
        {
            uart_write_char(code);
            put_newline();
        }
        __delay_cycles(50000);
    }
}
