#include "pwm.h"
#include <msp430fr5969.h>

void pwm_init(int duty_cycle_1, int duty_cycle_2)
{
    P1DIR |= BIT2;              // pin 1.2
    P1SEL0 |= BIT2;

    TA1CCR0 = 2000;                           // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = duty_cycle_1;                   // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = duty_cycle_2;                   // CCR2 PWM duty cycle
    TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;  // SMCLK, up mode, clear TAR
}
