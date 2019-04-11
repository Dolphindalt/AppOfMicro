#include "adc.h"
#include <msp430fr5969.h>

volatile unsigned int result;

void adc_init()
{
    P1SEL0 |= BIT2;                           // configure P1.2/A2 for ADC function
    P1SEL1 |= BIT2;                           //

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure ADC12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;        // Sampling time, S&H=16, ADC12 on
    ADC12CTL1 = ADC12SHP;                     // Use sampling timer
    ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
    ADC12MCTL0 |= ADC12INCH_1;                // A1 ADC input select; Vref=AVCC
}

void adc_start_sample()
{
    ADC12CTL0 |= ADC12ENC | ADC12SC;
    result = ADC12MEM0;
}

unsigned int adc_get_result()
{
    return result;
}
