/**
 * @author Dalton Caron
 * @version 2/2/209
 */
#include<msp430.h>
#include "keypad.h"

#define INPUT_PINS (BIT2 | BIT3 | BIT4 | BIT5)

static char reading = 0;
static char global_char = '\0';
static const int cycles[4] = { BIT0 | BIT1, BIT1, BIT0, 0 };
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

    // pull 1ne low on 2.0
    P2DIR |= BIT0;
    P2OUT &=~(BIT0);

    // nA0 -> 1.0
    P1DIR |= BIT0;

    // nA1 -> 1.1
    P1DIR |= BIT1;
}

int keypad_read_char(char *c)
{
    if(reading == 1)
    {
        *c = global_char;
        reading = 0;
        __delay_cycles(50000);
        return 1;
    }
    return 0;
}

void keypad_poll()
{
    char r, c;
    P1OUT &=~(BIT0|BIT1);
    for(c = 0; c < 4; c++)
    {
        P1OUT |= cycles[c];

        for(r = 0; r < 4; r++)
        {
            if(!(P1IN & inputs[r]))
            {
                while(!(P1IN & inputs[r])); // debounce
                global_char = keymap[r][c];
                reading = 1;
                return;
            }
        }
        P1OUT &=~(BIT0|BIT1);
    }
}
