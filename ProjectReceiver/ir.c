#include "ir.h"
#include <msp430fr5969.h>

void ir_init()
{
    P1DIR |= BIT0;
    P1OUT &=(~BIT0);
    P1IE |= BIT4;
    P1IFG &=(~BIT4);
}

#pragma vector=PORT1_VECTOR
__interrupt void interruptPort2()
{
    P1OUT |= BIT0;
    __no_operation();
    P1OUT &=(~BIT0);
    P1IFG &=(~BIT4);
}
