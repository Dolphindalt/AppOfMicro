#include <msp430fr5969.h>
#include "uart.h"
#include "adc.h"

extern int temperatureC;
extern int temperatureF;

void set_duty_cycle();

void show_temperature();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &=~LOCKLPM5;
	uart_init();
	adc_temperature_init();
	_:
	uart_write_string("Welcome to lab 2. Please choose an option:\n\r");
    uart_write_string("Q - Give me the temperature\n\r");
    uart_write_string("P - Let me set the duty cycle\n\r");
    char c = uart_read_char();
    put_newline();
    switch(c)
    {
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
        uart_write_string("That does not do anything.");
        break;
    }
    put_newline();
	goto _;
}

void set_duty_cycle()
{

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
