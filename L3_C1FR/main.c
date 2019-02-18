/*
 * @author Dalton Caron
 * @version 2/2/209
 */
#include <msp430fr5969.h>
#include "pwm.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &=~LOCKLPM5;
	
    pwm_TA1_init(100, 0);

    unsigned int dc = 200;
    unsigned int ctr = 60000;
	_:
	    ctr++;
	    if(ctr == 60000)
	    {
	        dc += 100;
	        if(dc >= 1000)
	            dc = 100;
	        pwm_TA1_init(dc, 0);
	    }
	goto _;
}
