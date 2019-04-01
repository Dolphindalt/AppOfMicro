/**
 * @author Dalton
 *
 * MSP430G2253 byte keyboard master
 */
#include <msp430g2253.h>
#include "i2c.h"

#define FR_SLAVE_ADDRESS 0x50
#define FR_SLAVE_GET_BYTE 0x01

#define LCD_SLAVE_ADDRESS 0x01
#define LCD_SLAVE_SEND_BYTE 0x01

void initClockTo16MHz()
{
    if (CALBC1_16MHZ==0xFF)
    {
        while(1);
    }
    DCOCTL = 0;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	initClockTo16MHz();
	i2c_init();
	P1DIR |= BIT1;
	P1OUT &=(~BIT1);

	while(1)
	{
	    i2c_send_start();
	    i2c_send_address(LCD_SLAVE_ADDRESS, 1);
	    i2c_send_byte(LCD_SLAVE_SEND_BYTE);
	    i2c_send_byte('a');
	    i2c_send_end();
	    __delay_cycles(5000);
        /*i2c_send_start();
        i2c_send_byte(FR_SLAVE_ADDRESS | 0x01);
        i2c_send_byte(FR_SLAVE_GET_BYTE);
        char c = i2c_read_byte();
        i2c_send_end();
        i2c_send_start();
        i2c_send_byte(LCD_SLAVE_ADDRESS);
        i2c_send_byte(LCD_SLAVE_SEND_BYTE);
        i2c_send_byte(c);
        i2c_send_end();*/
	}
}
