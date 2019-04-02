/**
 * @author Dalton
 *
 * MSP430G2253 byte keyboard master
 */
#include <msp430g2253.h>
#include "i2c.h"

#define FR_SLAVE_ADDRESS 0x50
#define FR_SLAVE_GET_BYTE 0x01

#define LCD_SLAVE_ADDRESS 0x60
#define LCD_SLAVE_SEND_BYTE 0x01

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	i2c_init();

	while(1)
	{
        i2c_send_start();
        i2c_send_byte(FR_SLAVE_ADDRESS | 0x01);
        i2c_send_byte(FR_SLAVE_GET_BYTE);
        char c = i2c_read_byte();
        i2c_send_end();
        i2c_send_start();
        i2c_send_byte(LCD_SLAVE_ADDRESS);
        i2c_send_byte(LCD_SLAVE_SEND_BYTE);
        i2c_send_byte(c);
        i2c_send_end();
	}
}
