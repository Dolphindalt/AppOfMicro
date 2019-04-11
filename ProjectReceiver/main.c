/**
 * Project Receiver
 * @author Dalton Caron
 */
#include <msp430fr5969.h>

int main()
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &=~LOCKLPM5;
	
	return 0;
}
