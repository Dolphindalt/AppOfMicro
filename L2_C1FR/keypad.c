#include<msp430fr5969.h>
#include "keypad.h"

#define INPUT_PINS (BIT2 | BIT3 | BIT4 | BIT5)

static const int cycles[4] = { BIT5 | BIT6, BIT6, BIT5, 0 };
static const int inputs[4] = { BIT2, BIT3, BIT4, BIT5 };
static const char keymap[4][4] = {
                             { '1', '2', '3', 'A' },
                             { '4', '5', '6', 'B' },
                             { '7', '8', '9', 'C' },
                             { '*', '0', '#', 'D' }
};

void keypad_init()
{
    // keypad inputs 1.3 1.4 1.5 1.2
    P1DIR &=~INPUT_PINS;
    P1REN |=INPUT_PINS;
    P1OUT |=INPUT_PINS;

    // pull 1ne low on 3.4
    P3DIR |= BIT4;
    P3OUT &=~(BIT4);

    // nA0 -> 3.5
    P3DIR |= BIT5;

    // nA1 -> 3.6
    P3DIR |= BIT6;

    //TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled
    //TA1CCR0 = 5000;
    //TA1CTL = TASSEL_1 | MC_2;

    //P1IE |= INPUT_PINS;

    //_BIS_SR(GIE);
}

char keypad_poll()
{
    char r, c;
    P3OUT &=~(BIT5|BIT6);
    for(c = 0; c < 4; c++)
    {
        P3OUT |= cycles[c];
        for(r = 0; r < 4; r++)
        {
            if(!(P1IN & inputs[r]))
            {
                while(!(P1IN & inputs[r]));
                return keymap[r][c];
            }
        }
        P3OUT &=~(BIT5|BIT6);
    }
    return 0;
}
