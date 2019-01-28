#include <msp430fr5969.h>

int main()
{
    PM5CTL0 &=(~LOCKLPM5);
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	// active low pin
	P3DIR |= 0x10;              // set 3.4 to output
	P3OUT &=~(0x10);            // set 3.4 low

	// nA0 pin
	P4DIR |= 0x20;              // 4.5
	P4OUT &=~(0x20);

	// nA1 pin
	P3DIR |= 0x02;              // 3.1
	P3OUT &=~(0x02);

	// keypad inputs 1.3 1.4 1.5 1.7
	P1DIR &=~(0x80 | 0x08 | 0x10 | 0x20);

	while(1);
}
