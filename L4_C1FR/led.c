#include <msp430fr5969.h>
#include "led.h"
#include "i2c.h"

char map['g'];

void led_init()
{
    map['0'] = 0x7E; // constant lookup times at the cost of memory
    map['1'] = 0x30;
    map['2'] = 0x6D;
    map['3'] = 0x79;
    map['4'] = 0x33;
    map['5'] = 0x5B;
    map['6'] = 0x5F;
    map['7'] = 0x70;
    map['8'] = 0x7F;
    map['9'] = 0x7B;
    map['A'] = 0x77;
    map['a'] = 0x77;
    map['B'] = 0x1F;
    map['b'] = 0x1F;
    map['C'] = 0x4E;
    map['c'] = 0x4E;
    map['D'] = 0x3D;
    map['d'] = 0x3D;
    map['E'] = 0x4F;
    map['e'] = 0x4F;
    map['f'] = 0x47;
    map['F'] = 0x47;
}

void led_send_chars(char sub, char cbs, char *c)
{
    char ctr = 0;
    i2c_send_start();
    i2c_send_byte(SLAVE_ADDRESS);
    i2c_send_byte(sub);
    i2c_send_byte(cbs);
    while(ctr < 4 && c[ctr] != '\0')
    {
        i2c_send_byte(map[c[ctr++]]);
    }
    i2c_send_end();
}
