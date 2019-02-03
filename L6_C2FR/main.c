#include <msp430fr5969.h>
#include "uart.h"
#include "adc.h"
#include "keypad.h"
#include "pwm.h"

extern int temperatureC;
extern int temperatureF;

void set_duty_cycle();

void show_temperature();

// returns 1 if success or -1 if overflow
int build_keypad_string(char *buff, unsigned int size);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &=~LOCKLPM5;
	keypad_init();
	uart_init();
	adc_temperature_init();
	pwm_init(750, 250);
	_:
	uart_write_string("Welcome to lab 2. Please choose an option:\n\r");
    uart_write_string("Q - Give me the temperature\n\r");
    uart_write_string("P - Let me set the duty cycle\n\r");
    uart_write_string("K - Keypad write mode\n\r");
    char c = uart_read_char();
    put_newline();
    switch(c)
    {
    case 'K':
        __no_operation();
        char buff[99];
        if(build_keypad_string(buff, 99))
            uart_write_string(buff);
        else
            uart_write_string("An error occurred while trying to read the keypad");
        break;
    case 'P':
        set_duty_cycle();
        break;
    case 'p':
        set_duty_cycle();
        break;
    case 'Q':
        show_temperature();
        break;
    case 'q':
        show_temperature();
        break;
    default:
        uart_write_string("That does not do anything");
        break;
    }
    put_newline();
	goto _;
}

void set_duty_cycle()
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
    pwm_init(new_crr0, new_crr1);
    uart_write_string("PWM set\n\r");
}

void show_temperature()
{
    adc_read_temperature();
    char temp_buffer[10]; // its a temporary buffer but also hold the temperature so it's a pun
    primitive_to_buffer(temp_buffer, 10, temperatureF);
    uart_write_string(temp_buffer);
    uart_write_char('F');
    put_newline();
    primitive_to_buffer(temp_buffer, 10, temperatureC);
    uart_write_string(temp_buffer);
    uart_write_char('C');
    put_newline();
}

int build_keypad_string(char *buff, unsigned int size)
{
    char c;
    unsigned int ptr = 0;
    while(1)
    {
        keypad_poll();
        if(keypad_read_char(&c))
        {
            if(c == '*') break;
            else if(c == '#')
            {
                if(ptr + 2 == size) return -1;
                buff[ptr++] = '\r';
                buff[ptr++] = '\n';
            }
            else
            {
                if(ptr == size) return -1;
                buff[ptr++] = c;
            }
        }
    }
    if(ptr == size) return -1;
    buff[ptr] = '\0';
    return 1;
}
