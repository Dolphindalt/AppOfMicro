#include <msp430fr5969.h>
#include <stdint.h>
 // 1.7 clock 1.6 sda
#define SLAVE_ADDRESS 0x50
#define pollZeroButton() (!(P4IN & BIT5 == BIT5))
#define pollOneButton() (!(P1IN & BIT1 == BIT1))

#define SLAVE_CMD_0 0 // have slave send address
#define SLAVE_CMD_0_LEN 1
#define SLAVE_CMD_1 1 // have slave take input and send byte
#define SLAVE_CMD_1_LEN 1

#define MAX_BUFFER_SIZE 20

typedef enum I2C_ModeEnum{
    IDLE_MODE,
    NACK_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    SWITCH_TO_RX_MODE,
    SWITHC_TO_TX_MODE,
    TIMEOUT_MODE
} I2C_Mode;

I2C_Mode SlaveMode = RX_REG_ADDRESS_MODE;
uint8_t ReceiveRegAddr = 0;
uint8_t ReceiveBuffer[MAX_BUFFER_SIZE] = {0};
uint8_t RXByteCtr = 0;
uint8_t ReceiveIndex = 0;
uint8_t TransmitBuffer[MAX_BUFFER_SIZE] = {0};
uint8_t TXByteCtr = 0;
uint8_t TransmitIndex = 0;

uint8_t SlaveCmd0Buf[SLAVE_CMD_0_LEN] = {SLAVE_ADDRESS};
uint8_t SlaveCmd1Buf[SLAVE_CMD_1_LEN] = {0};

void init();
void buttonInput();
void I2C_Slave_ProcessCMD(uint8_t cmd);
void I2C_Slave_TransactionDone(uint8_t cmd);
void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);

int main(void)
{
    PM5CTL0 &=(~LOCKLPM5);
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	return 0;
}

void init()
{
    CSCTL0_H = CSKEY >> 8; // jump to 16MHz
    CSCTL1 = DCORSEL | DCOFSEL_4;
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL0_H = 0;
    P4DIR &=(~BIT5); // init the two buttons, active low
    P4REN |= BIT5;
    P4OUT |= BIT5;
    P1DIR &=(~BIT1);
    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1SEL1 |= BIT6 | BIT7; // I2C pins and stuff
    UCB0CTLW0 = UCSWRST;
    UCB0CTLW0 |= UCMODE_3 | UCSYNC;
    UCB0I2COA0 = SLAVE_ADDRESS | UCOAEN;
    UCB0CTLW0 &= ~UCSWRST;
    UCB0IE |= UCRXIE + UCSTPIE;
    FRCTL0 = FRCTLPW | NWAITS_1;
}

void buttonInput()
{
    int ctr = 0;
    while(ctr < 8)
    {
        if(pollZeroButton())
        {
            SlaveCmd1Buf[0] = 0;
            SlaveCmd1Buf[0] <<= 1;
            ctr++;
        }
        else if(pollOneButton())
        {
            SlaveCmd1Buf[0] = 1;
            SlaveCmd1Buf[0] <<= 1;
            ctr++;
        }
    }
    __no_operation();
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  //Must read from UCB0RXBUF
  uint8_t rx_val = 0;
  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_I2C_UCSTPIFG:
        UCB0IFG &= ~(UCTXIFG0);
        break;         // Vector 8: STPIFG
    case USCI_I2C_UCRXIFG0:                 // Vector 22: RXIFG0
        rx_val = UCB0RXBUF;
        switch (SlaveMode)
        {
          case (RX_REG_ADDRESS_MODE):
              ReceiveRegAddr = rx_val;
              I2C_Slave_ProcessCMD(ReceiveRegAddr);
              break;
          case (RX_DATA_MODE):
              ReceiveBuffer[ReceiveIndex++] = rx_val;
              RXByteCtr--;
              if (RXByteCtr == 0)
              {
                  //Done Receiving MSG
                  SlaveMode = RX_REG_ADDRESS_MODE;
                  UCB0IE &= ~(UCTXIE);
                  UCB0IE |= UCRXIE;                          // Enable RX interrupt
                  I2C_Slave_TransactionDone(ReceiveRegAddr);
              }
              break;
          default:
              __no_operation();
              break;
        }
        break;
    case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
        switch (SlaveMode)
        {
          case (TX_DATA_MODE):
              UCB0TXBUF = TransmitBuffer[TransmitIndex++];
              TXByteCtr--;
              if (TXByteCtr == 0)
              {
                  //Done Transmitting MSG
                  SlaveMode = RX_REG_ADDRESS_MODE;
                  UCB0IE &= ~(UCTXIE);
                  UCB0IE |= UCRXIE;                          // Enable RX interrupt
                  I2C_Slave_TransactionDone(ReceiveRegAddr);
              }
              break;
          default:
              __no_operation();
              break;
        }
        break;                      // Interrupt Vector: I2C Mode: UCTXIFG
    default: break;
  }
}

void I2C_Slave_ProcessCMD(uint8_t cmd)
{
    switch(cmd)
    {
    case SLAVE_CMD_0:
        SlaveMode = TX_DATA_MODE;
        TXByteCtr = SLAVE_CMD_0_LEN;
        CopyArray(SlaveCmd0Buf, TransmitBuffer, SLAVE_CMD_0_LEN);
        UCB0IE &= ~UCRXIE;
        UCB0IE |= UCTXIE;
        break;
    case SLAVE_CMD_1:
        SlaveMode = TX_DATA_MODE;
        TXByteCtr = SLAVE_CMD_1_LEN;
        CopyArray(SlaveCmd1Buf, TransmitBuffer, SLAVE_CMD_1_LEN);
        UCB0IE &= ~UCRXIE;
        UCB0IE |= UCTXIE;
        break;
    default:
        break;
    }
}

void I2C_Slave_TransactionDone(uint8_t cmd)
{
    switch(cmd)
    {
    case SLAVE_CMD_0:
        break;
    case SLAVE_CMD_1:
        break;
    default:
        break;
    }
}

void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
    int i;
    for(i = 0; i < count; i++)
        dest[i] = source[i];
}
