/**
 * @author Dalton Caron
 * @version 1/20/2019
 */
#include "uart.h"
#include <msp430fr5969.h>

#define READ_BUFFER_MAX 100
#define WRITE_BUFFER_MAX 100

static char read_buffer[READ_BUFFER_MAX];
static unsigned int read_buffer_offset = 0;
static char write_buffer[WRITE_BUFFER_MAX];
static unsigned int write_buffer_offset = 0;

static char rx_char;

#define PASSIVE_MODE 0
#define BUFFERED_READ_MODE 1
#define BUFFERED_WRITE_MODE 2
#define SINGLE_BYTE_READ_MODE 3

/**
 * Modes
 * 0 - Passive mode allows a user to rx what they tx.
 * 1 - Reads key inputs into buffer until terminated by newline.
 * 2 - Writing content in the write buffer, unable to process input.
 * 3 - Single byte read mode.
 */
unsigned int mode = PASSIVE_MODE;

void uart_init()
{
    P2SEL1 |= BIT0 | BIT1;                    // Configure UART pins
    P2SEL0 &= ~(BIT0 | BIT1);
    CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers
    UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
    UCA0BR0 = 52;
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1;
    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE|UCTXCPTIE;               // Enable USCI_A0 RX interrupt
    __bis_SR_register(GIE);                   // Interrupts enabled
    __no_operation();                         // For debugger
}

char uart_read_char()
{
    mode = SINGLE_BYTE_READ_MODE;
    while(mode == SINGLE_BYTE_READ_MODE);
    return rx_char;
}

void uart_write_char(const char c)
{
    while(!(UCA0IFG&UCTXIFG));
    UCA0TXBUF = c;
}

char *uart_read_string()
{
    mode = BUFFERED_READ_MODE;
    read_buffer_offset = 0;
    while(mode == BUFFERED_READ_MODE);
    return read_buffer;
}

void uart_write_string(const char *string)
{
    unsigned int c = 0;
    while(string[c] ^ '\0')
    {
        write_buffer[c] = string[c];
        c++;
    }
    write_buffer[c] = '\0';
    write_buffer_offset = 0;
    mode = BUFFERED_WRITE_MODE;
    while(write_buffer[write_buffer_offset] ^ '\0')
    {
        while(!(UCA0IFG&UCTXIFG));
        UCA0TXBUF = write_buffer[write_buffer_offset++];
    }
    mode = PASSIVE_MODE;
}

void put_newline()
{
    uart_write_char('\n');
    uart_write_char('\r');
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

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR()
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      if(mode == BUFFERED_READ_MODE)            // read mode
      {
          while(!(UCA0IFG&UCTXIFG));
          char c = UCA0RXBUF;
          if(c == '\n' || c == '\r' || c == '\0')
          {
              read_buffer[read_buffer_offset] = '\0';
              mode = PASSIVE_MODE;
              return;
          }
          read_buffer[read_buffer_offset++] = c;
          UCA0TXBUF = c;
      }
      else if(mode == 3)                        // single char read mode
      {
          while(!(UCA0IFG&UCTXIFG));
          rx_char = UCA0RXBUF;
          UCA0TXBUF = rx_char;
          mode = PASSIVE_MODE;
      }
      else if(mode == PASSIVE_MODE)             // passive mode
      {
          while(!(UCA0IFG&UCTXIFG));
              UCA0TXBUF = UCA0RXBUF;
      }
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
}
