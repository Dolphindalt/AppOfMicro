#include "vu.h"
#include "spi.h"
#include "lcd.h"
#include "adc.h"

#define DAS 50
#define AMP 10

int digital_values[DAS];
unsigned long average;

void vu_init()
{
    spi_init();
    lcd_init();
    adc_init();
}

void vu_tick()
{
    unsigned int i;
    average = 0;
    for(i = 0; i < DAS; i++)
    {
        adc_start_sample();
        digital_values[i] = AMP*adc_get_result();
        average += digital_values[i];
    }
    average /= DAS;
    for(i = 0; i < DAS; i++)
    {
        digital_values[i] -= average;
        if(digital_values[i] < 0)
            digital_values[i] *= -1;
    }
    average = 0;
    for(i = 0; i < DAS; i++)
    {
        average += digital_values[i];
    }
    average /= DAS;
}

void vu_display()
{
    unsigned int vm = (average * 16) / 1023;
    char i;
    lcd_set_ddram(0x00);
    for(i = 0; i < vm; i++)
    {
        lcd_write_character(0xFC);
    }
    for(i = 0; i < 16; i++)
    {
        lcd_write_character(0x01);
    }
    char buf[5];
    primitive_to_buffer(buf, 5, average);
    lcd_set_ddram(0x40);
    for(i = 0; buf[i] != '\0'; i++)
    {
        lcd_write_character(buf[i]);
    }
    for(; i < 16; i++)
    {
        lcd_write_character(0x01);
    }
}

int primitive_to_buffer(char *buffer, unsigned int buffer_size, int primitive)
{
    if(buffer_size < 2) return 0;
    if(primitive == 0)                          // test special case
    {
            buffer[0] = '0';
            buffer[1] = '\0';
            return 1;
    }
    unsigned int buffer_ptr = 0, i;
    int negative = primitive < 0;
    unsigned int n = negative ? -primitive : primitive;
    while(n)                                    // calculate each digit
    {
        if(buffer_ptr >= buffer_size) return 0;
        buffer[buffer_ptr++] = n % 10 + '0';
        n /= 10;
    }
    if(negative)                                // include the minus if negative
    {
        if(buffer_ptr >= buffer_size) return 0;
        buffer[buffer_ptr++] = '-';
    }
    if(buffer_ptr >= buffer_size) return 0;
    buffer[buffer_ptr] = '\0';
    for(i = 0; i < buffer_ptr/2; i++)           // swap the order of the string
    {
        buffer[i] ^= buffer[buffer_ptr-i-1];
        buffer[buffer_ptr-i-1] ^= buffer[i];
        buffer[i] ^= buffer[buffer_ptr-i-1];
    }
    return 1;
}

