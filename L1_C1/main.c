/**
 * L1_C1
 * @author Dalton Caron
 */
#include <msp430.h> 

#define LED1 0x01
#define LED2 0x40

int main()
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= LED1 | LED2;       // Set 0.0 and 0.6 to outputs
	P1OUT &=(~LED2);            // Clear 0.6 low
	P1OUT |= LED1;              // Set 0.0 high
	while(1)
	{
	    P1OUT ^= LED1 | LED2;   // toggle the LEDs
	    __delay_cycles(66667);  // delay = 10^6 / 15
	}
}
