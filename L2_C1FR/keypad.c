#include<msp430fr5969.h>
#include "keypad.h"

#define INPUT_PINS (BIT2 | BIT3 | BIT4 | BIT5)

static char reading = 0;
static char global_char = 0;
static char lazy_index = 0;
static const int cycles[4] = { BIT3 | BIT0, BIT3, BIT0, 0 }; // 0 1 2 3
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
    P1OUT |=~INPUT_PINS;

    // pull 1ne low on 3.4
    P3DIR |= BIT4;
    P3OUT &=~(BIT4);

    // nA0 -> 3.0
    P3DIR |= BIT0;

    // nA1 -> 4.3
    P4DIR |= BIT3;

    TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled
    TA1CCR0 = 5000;
    TA1CTL = TASSEL_1 | MC_2;

    P1IE |= INPUT_PINS;

    _BIS_SR(GIE);
}

int keypad_read_char(char *c)
{
    if(reading == 1)
    {
        *c = global_char;
        reading = 0;
        return 1;
    }
    return 0;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{
    P3OUT &=~(BIT0);
    P4OUT &=~(BIT3);
    P3OUT |= (cycles[lazy_index]&BIT0);
    P4OUT |= (cycles[lazy_index]&BIT3);
    lazy_index++;
    if(lazy_index == 4) lazy_index = 0;
    TA1CCR0 += 100;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1()
{
    reading = 1;
    switch(_even_in_range(P1IV,16))
    {
    case P1IV_P1IFG2: // 0
        global_char = keymap[lazy_index][0];
        P1IFG &=~(BIT2);
        break;
    case P1IV_P1IFG3: // 1
        global_char = keymap[lazy_index][1];
        P1IFG &=~(BIT3);
        break;
    case P1IV_P1IFG4: // 2
        global_char = keymap[lazy_index][2];
        P1IFG &=~(BIT4);
        break;
    case P1IV_P1IFG5: // 3
        global_char = keymap[lazy_index][3];
        P1IFG &=~(BIT5);
        break;
    default:
        P1IFG &=~(0xD3);
        reading = 0;
        break;
    }

}
