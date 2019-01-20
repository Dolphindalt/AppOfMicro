#ifndef UART_H
#define UART_H

void uart_init();

char uart_read_char();

void uart_write_char(const char);

char *uart_read_string();

void put_newline();

void uart_write_string(const char *);

#endif
