/**
 * This header file provides general purpose uart communication functions.
 * @author Dalton Caron
 * @version 1/20/2019
 */
#ifndef UART_H
#define UART_H

/**
 * Initializes uart for 8MHz.
 */
void uart_init();

/**
 * Reads a single character from uart.
 * @return The character read from uart.
 */
char uart_read_char();

/**
 * Writes a string to uart.
 * @param The character to write out.
 */
void uart_write_char(const char);

/**
 * Reads a string from uart that is terminated by a \r, \n, or \0.
 * Note that each use of this function disregards the previous buffered string return by this function.
 */
char *uart_read_string();

/**
 * Sends a \n and \r through uart.
 */
void put_newline();

/**
 * Writes a string through uart.
 * @param The string to write, terminated by \0.
 */
void uart_write_string(const char *);

/**
 * Converts a primitive signed integer type into a string.
 * @param char * The buffer to write the string to.
 * @param unsigned int The size of the buffer.
 * @param int The numerical value to translate into string form.
 */
int primitive_to_buffer(char *, unsigned int, int);

#endif
