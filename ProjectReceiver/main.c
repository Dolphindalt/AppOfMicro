/**
 * Project Receiver
 * @author Dalton Caron
 */
#include <msp430fr5969.h>
#include "ir.h"

int main()
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &=~LOCKLPM5;
	
    ir_init();

    __bis_SR_register(GIE);

	while(1);
}
