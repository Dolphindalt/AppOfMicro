#include <msp430fr5969.h>
#include "keypad.h"

static char busy = 0;
static char val = 0;
static char count = 0;
static char edge = 0;

int main()
{
    PM5CTL0 &=(~LOCKLPM5);
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT0;              // 1.1 green led
    P4DIR |= BIT6;              // 4.6 red led
    P4OUT &=~(BIT6);
    P1OUT &=~(BIT0);

    keypad_init();
    _BIS_SR(GIE);

    volatile char hex_map[0x48];
    volatile char i;
    for(i = 0; i < 10; i++)
        hex_map['0'+i] = i;     // behold my c code (¬‿¬)
    for(i = 'A'; i < 'E'; i++)
        hex_map[i] = i;         // for clarification
    hex_map['#'] = '#';
    hex_map['*'] = '*';

    while(1)
    {
        char c;
        keypad_poll();
        char code = keypad_read_char(&c);
        if(code && !busy)
        {
            busy = 1;
            count = 8;
            val = hex_map[c];
            TA1CCTL0 |= CCIE;
            TA1CCR0 = 0;
            TA1CTL |= TASSEL_2 | MC_2;
        }
    }
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR()
{
    P1OUT &=~(BIT0);
    if(edge == 0x01) // high edge, clock bit
    {
        P4OUT |= BIT6;
        if(count ^ 0)
        {
            volatile char t = val;
            volatile char d = (val >> (count-1)) & BIT0;
            P1OUT |= (val >> (count-1)) & BIT0;
            count--;
        }
        else
        {
            TA1CCTL0 &=~(CCIE);
            busy = 0;
            P1OUT &=~(BIT0);
            P4OUT &=~(BIT6);
        }
    }
    else // low edge, turn off clock and data
    {
        P4OUT &=~(BIT6);
        P1OUT &=~(BIT0);
    }
    edge ^= 0x01;
    TA1CCR0 += 60000;
}
