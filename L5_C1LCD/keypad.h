/**
 * @author Dalton Caron
 * @version 2/2/209
 */
#ifndef KEYPAD_H
#define KEYPAD_H

/**
 * Keypad hookups
 * 4 -> Y3
 * 3 -> Y2
 * 2 -> Y1
 * 1 -> Y0
 * 1.2 -> 8
 * 1.3 -> 7
 * 1.4 -> 6
 * 1.5 -> 5
 * 2.3 -> E0
 * 2.4 -> A0
 * 2.5 -> A1
 */

void keypad_init();

int keypad_read_char(char *);

void keypad_poll();

#endif
