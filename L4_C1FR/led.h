#ifndef LED_H
#define LED_H

#define SLAVE_ADDRESS 0x70
#define DISPLAY_FOUR_CONTROL_BITS 0x46

void send_char_to_digit(char sub, char cbs, char *c);

#endif
