#ifndef LED_H
#define LED_H

#define SLAVE_ADDRESS 0x76
#define CONTROL 0x77

void led_init();

void led_send_chars(char sub, char cbs, char *c);

#endif
