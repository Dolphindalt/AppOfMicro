#include <msp430.h> 

#define BUTTON 0x08
#define LED1 0x01
#define LED2 0x40

void blink_when_button_held()
{
    if(!(P1IN & BUTTON))
    {
        P1OUT ^= LED1 | LED2;
    }
}

void blink_when_button_not_held()
{
    if(P1IN & BUTTON)
    {
        P1OUT ^= LED1 | LED2;
    }
}

void random_game(int *ctr)
{
    while(!(P1IN & BUTTON));
    *ctr = (*ctr ^ 3) ? *ctr+1 : 0;
    P1OUT &=(~(LED1|LED2));
    switch(*ctr)
    {
    case 0:
        break;
    case 1:
        P1OUT |= LED2;
        break;
    case 2:
        P1OUT |= LED1;
        break;
    case 3:
        P1OUT |= LED1 | LED2;
        break;
    }
}

int main()
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR &=(~BUTTON);          // set 0.3 to an input
	P1REN |= BUTTON;            // enable resistor for 0.3
	P1OUT |= BUTTON;            // set 0.3 to a pull up resistor

	P1DIR |= LED1 | LED2;       // enable LEDs as output
	P1OUT |= 0x01;              // set 0.0 high

	int ctr = 0;

	while(1)
	{
	    random_game(&ctr);
	    __delay_cycles(66667);  // delay = 10^6 / 15
	}
}
