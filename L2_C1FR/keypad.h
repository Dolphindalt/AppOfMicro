#ifndef KEYPAD_H
#define KEYPAD_H

/**
 * 4 -> Y3
 * 3 -> Y2
 * 2 -> Y1
 * 1 -> Y0
 * 1.2 -> 8
 * 1.3 -> 7
 * 1.4 -> 6
 * 1.5 -> 5
 * 3.4 -> E0
 * 3.5 -> A0
 * 3.6 -> A1
 */

void keypad_init();

char keypad_poll();

int keypad_read_char(char *);

#endif
