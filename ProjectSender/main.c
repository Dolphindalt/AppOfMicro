/**
 * Project Sender
 * @author Dalton Caron
 */
#include <msp430fr5969.h>
#include "adc.h"
#include "dac.h"
#include "wavelet.h"
#include "ied.h"

#define DS_LEN 250
#define AMP 1
#define CLK 16
#define PERIOD (250 * CLK)

unsigned int gctr = 0;
float digital_signal[DS_LEN];
unsigned long average = 0;

void sender_init();

int main()
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &=~LOCKLPM5;
	
    sender_init();

    TA0CCTL0 = CCIE;
    gctr = 0;
    while(gctr < DS_LEN); // wait until signal is recorded

    int c = 0;
    while(++c != 0) // play sound for a bit
    {
        int i;
        for(i = 0; i < DS_LEN; i++)
            dac_set_voltage(digital_signal[i], 0);
    }

    // wavelet
    flegall53(digital_signal, DS_LEN);

    // transmit
    while(1);
}

void sender_init()
{
    FRCTL0 = FRCTLPW | NWAITS_1;

    // Clock System Setup
    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL1 = DCORSEL | DCOFSEL_4;             // Set DCO to 16MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK; // Set SMCLK = MCLK = DCO,
                                                // ACLK = VLOCLK
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers
    TA0CCTL0 = CCIE;
    TA0CCR0 = PERIOD;
    TA0CTL = TASSEL_2 + MC_2;
    adc_init();
    dac_init();
    __bis_SR_register(GIE);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A()
{
    if(gctr < DS_LEN)
    {
        adc_start_sample();
        digital_signal[gctr] = AMP*(adc_get_result() << 2);
        gctr++;
    }
    else
    {
        TA0CCTL0 &=(~CCIE);
    }
    TA0CCR0 += PERIOD;
}
