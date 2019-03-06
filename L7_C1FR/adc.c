#include "adc.h"
#include <msp430fr5969.h>

volatile unsigned int result;

void adc_init()
{
    P1SEL1 |= BIT2;                    // Configure ADC input A2
    P1SEL0 |= BIT2;

    ADC12CTL0 = ADC12SHT0_15 | ADC12ON;       // Sampling time, ADC12 on
    ADC12CTL1 = ADC12SHP;                     // Use sampling timer
    ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
    ADC12MCTL0 |= ADC12INCH_2 | ADC12DIF; // Channel2 ADC input select; Vref=AVCC
    ADC12IER0 |= ADC12IE0;                 // Enable ADC conv complete interrupt
}

void adc_start_sample()
{
    ADC12CTL0 |= ADC12ENC | ADC12SC;
}

unsigned int adc_get_result()
{
    return result;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
    {
    case ADC12IV_ADC12IFG0:                 // Vector 12:  ADC12MEM0 Interrupt
        result = ADC12MEM0;                 // read out the result register
        break;                                // Clear CPUOFF bit from 0(SR)
    default:
        break;
    }
}
