#include <msp430.h>
#include "vu.h"

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	__bis_SR_register(GIE);
	
	vu_init();

	while(1);
}
