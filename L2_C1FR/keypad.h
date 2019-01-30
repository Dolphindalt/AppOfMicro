#ifndef KEYPAD_H
#define KEYPAD_H

/**
 * keypad 5-8 maps to 1.2-1.5
 */

void keypad_init();

int keypad_read_char(char *);

#endif
