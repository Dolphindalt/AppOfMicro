#include <msp430.h>
#include "game.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    __bis_SR_register(GIE);

    while(1)
    {
        game_loop();
    }
}
