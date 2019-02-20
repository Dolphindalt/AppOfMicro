#ifndef LED_H
#define LED_H

#define SLAVE_ADDRESS 0x76
#define CONTROL 0x77

void send_char_to_digit(char sub, char cbs, char *c);

#endif
