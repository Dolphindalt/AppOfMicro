#include <msp430fr5969.h>
#include "led.h"
#include "i2c.h"

void send_char_to_digit(char sub, char cbs, char *c)
{
    char ctr = 0;
    i2c_send_start();
    i2c_send_byte(SLAVE_ADDRESS);
    i2c_send_byte(sub);
    i2c_send_byte(cbs);
    while(ctr < 4 && c[ctr++] != '\0')
    {
        i2c_send_byte(c[ctr]);
    }
    i2c_send_end();
}
