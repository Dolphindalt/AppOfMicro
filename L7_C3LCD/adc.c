#include "adc.h"
#include <msp430.h>

volatile unsigned int result;

void adc_init()
{
    ADC10CTL0 = ADC10SHT_2 + ADC10ON;         // ADC10ON
    ADC10CTL1 = INCH_1;                       // input A1
    ADC10AE0 |= BIT1;                         // PA.1 ADC option select
}

void adc_start_sample()
{
    ADC10CTL0 |= ENC + ADC10SC;
    result = ADC10MEM;
}

unsigned int adc_get_result()
{
    return result;
}
