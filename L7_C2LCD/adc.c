#include "adc.h"
#include <msp430.h>

volatile unsigned int result;

void adc_init()
{
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10CTL1 = INCH_1;                       // input A1
    ADC10AE0 |= BIT1;                         // PA.1 ADC option select
}

void adc_start_sample()
{
    ADC10CTL0 |= ENC + ADC10SC;
}

unsigned int adc_get_result()
{
    return result;
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    result = ADC10MEM;
}
