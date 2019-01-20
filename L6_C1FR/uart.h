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

#endif
