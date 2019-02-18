/*
 * @author Dalton Caron
 * @version 2/2/209
 */
#include "pwm.h"
#include <msp430fr5969.h>

void pwm_TA1_init(int duty_cycle_1, int duty_cycle_2)
{
    duty_cycle_1 *= 8;
    duty_cycle_2 *= 8;
    P1DIR |= BIT2;              // pin 1.2
    P1SEL0 |= BIT2;

    TA1CCR0 = 2000 * 8;                           // PWM Period
    TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA1CCR1 = duty_cycle_1;                   // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA1CCR2 = duty_cycle_2;                   // CCR2 PWM duty cycle
    TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;  // SMCLK, up mode, clear TAR
}

void pwm_B0_init(int duty_cycle_1, int duty_cycle_2)
{
    P1DIR |= BIT4;
    P1SEL0 |= BIT4;
    P1SEL1 &=~(BIT4);

    TB0CCR0 = 2000;
    TB0CCTL1 = OUTMOD_7;
    TB0CCR1 = duty_cycle_1;
    TB0CCTL2 = OUTMOD_7;
    TB0CCR2 = duty_cycle_2;
    TB0CTL = TASSEL__SMCLK | MC__UP | TACLR;
}
