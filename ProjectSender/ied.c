#include "ied.h"
#include <msp430fr5969.h>

#define IED BIT4
#define ENABLE_IED P1OUT |= BIT4
#define DISABLE_IED P1OUT &=(~BIT4)

// bit 0 time
#define BIT0_MIN 8000
#define BIT0_MAX 12800

// bit 1 time
#define BIT1_MIN 24000
#define BIT1_MAX 28800

void ied_init()
{
    P1DIR |= BIT4;
    DISABLE_IED;
}

void ied_send_high_bit()
{
    ENABLE_IED;
    __delay_cycles(BIT1_MIN);
}

void ied_send_low_bit()
{

}
