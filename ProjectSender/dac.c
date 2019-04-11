#include "dac.h"
#include "i2c.h"
#include <msp430fr5969.h>

#define ADDRESS 0xC0

void dac_init()
{
    i2c_init();
}

// assumes last two bits are 0
void dac_send_values(int *dv, unsigned int n)
{
    unsigned int i = 0;
    unsigned char t1, t2;
    while(i < n)
    {
        i2c_send_start();
        i2c_send_byte(ADDRESS);
        t1 = (dv[i] & 0x00FF);
        t1 &=~(0xC0);
        t2 = (dv[i] & 0xFF00) >> 8;
        i2c_send_byte(t1);
        i2c_send_byte(t2);
        i2c_send_end();
        i++;
    }
}
