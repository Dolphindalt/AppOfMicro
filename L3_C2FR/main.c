/*
 * @author Dalton Caron
 * @version 2/2/209
 */
#include <msp430fr5969.h>
#include "pwm.h"
#include "uart.h"
#include "keypad.h"

void modify_pwm(char c);

int main()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &=~LOCKLPM5;

    keypad_init();
    uart_init();

    pwm_TA1_init(160, 0);
    pwm_B0_init(160, 0);

    char c;

    _:
    c = uart_read_char();
    if(c)
    {
        if(c != '1' && c != '2') goto _;
        modify_pwm(c);
    }
    goto _;
}

void modify_pwm(char c)
{
    unsigned int new_crr0, new_crr1;
    char *s;
    uart_write_string("Provide a CCR1 value\n\r");
    s = uart_read_string();
    buffer_to_primitive(s, &new_crr0);
    put_newline();
    uart_write_string("Provide a CCR2 value\n\r");
    s = uart_read_string();
    buffer_to_primitive(s, &new_crr1);
    put_newline();
    if(c == '1')
    {

        pwm_TA1_init(new_crr0, new_crr1);
        uart_write_string("PWM A1 set\n\r");
    }
    else if(c == '2')
    {
        pwm_B0_init(new_crr0, new_crr1);
        uart_write_string("PWM B0 set\n\r");
    }
}
