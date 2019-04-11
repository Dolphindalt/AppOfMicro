/**
 * Project Sender
 * @author Dalton Caron
 */
#include <msp430fr5969.h>
#include "adc.h"
#include "dac.h"

#define DS_LEN 500
#define AMP 1
#define CLK 16
#define PERIOD (250 * CLK)

unsigned int gctr = 0;
int digital_signal[DS_LEN];
unsigned long average = 0;

void sender_init();

int main()
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &=~LOCKLPM5;
	
    sender_init();

    forever:
    TA0CCTL0 = CCIE;
    gctr = 0;
    while(gctr < DS_LEN);

    dac_send_values(digital_signal, DS_LEN);

    goto forever;

	return 0;
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
