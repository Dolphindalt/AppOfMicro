#include "ied.h"
#include <msp430fr5969.h>
#include "uart.h"

void ied_init()
{
    uart_init();

    P1DIR |= BIT2;                            // pin 1.2
    P1SEL0 |= BIT2;

    TA1CCR0 = 421;                            // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = 210;                            // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = 0;                              // CCR2 PWM duty cycle
    TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;  // SMCLK, up mode, clear TAR
}
