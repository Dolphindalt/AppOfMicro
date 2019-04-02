#include <msp430g2553.h>
#include <stdint.h>
#include "spi.h"
#include "lcd.h"

#define SLAVE_ADDR  0x60

#define LCD_SLAVE_CMD_0 0x01
#define LCD_SLAVE_CMD_0_LEN 1

#define MAX_BUFFER_SIZE     20

uint8_t MasterCmd0Buf[LCD_SLAVE_CMD_0_LEN];

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

void I2C_Slave_ProcessCMD(uint8_t cmd);
void I2C_Slave_TransactionDone(uint8_t cmd);
void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);

void I2C_Slave_ProcessCMD(uint8_t cmd)
{
    ReceiveIndex = 0;
    TransmitIndex = 0;
    RXByteCtr = 0;
    TXByteCtr = 0;

    switch (cmd)
    {
        case LCD_SLAVE_CMD_0:
            SlaveMode = RX_DATA_MODE;
            RXByteCtr = LCD_SLAVE_CMD_0_LEN;
            IE2 &= ~UCB0TXIE;
            IE2 |= UCB0RXIE;
            break;
        default:
            __no_operation();
            break;
    }
}


void I2C_Slave_TransactionDone(uint8_t cmd)
{
    switch (cmd)
    {
    case LCD_SLAVE_CMD_0:
        CopyArray(ReceiveBuffer, MasterCmd0Buf, LCD_SLAVE_CMD_0_LEN);
        break;
    default:
        __no_operation();
        break;
    }
}

void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
    uint8_t copyIndex = 0;
    for (copyIndex = 0; copyIndex < count; copyIndex++)
    {
        dest[copyIndex] = source[copyIndex];
    }
}

void initClockTo16MHz()
{
    if (CALBC1_16MHZ==0xFF)
    {
        while(1);
    }
    DCOCTL = 0;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
}

void initGPIO()
{
    P1SEL |= BIT6 + BIT7;
    P1SEL2|= BIT6 + BIT7;
}

void initI2C()
{
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
    UCB0I2COA = SLAVE_ADDR;                   // Own Address
    UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
    UCB0I2CIE |= UCSTPIE + UCSTTIE;           // Enable STT and STP interrupt
    IE2 |= UCB0RXIE;                          // Enable RX interrupt
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    initClockTo16MHz();
    initGPIO();
    initI2C();

    __bis_SR_register(GIE);

    spi_init();
    lcd_init();
    MasterCmd0Buf[0] = '\0';

    while(1)
    {
        if(MasterCmd0Buf[0] != '\0')
        {
            lcd_write_character(MasterCmd0Buf[0]);
            MasterCmd0Buf[0] = '\0';
        }
    }
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  if (IFG2 & UCB0RXIFG)                 // Receive Data Interrupt
  {
      //Must read from UCB0RXBUF
      uint8_t rx_val = UCB0RXBUF;
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
                  IE2 &= ~(UCB0TXIE);
                  IE2 |= UCB0RXIE;                          // Enable RX interrupt
                  I2C_Slave_TransactionDone(ReceiveRegAddr);
              }
              break;
          default:
              __no_operation();
              break;
      }

  }
  else if (IFG2 & UCB0TXIFG)            // Transmit Data Interrupt
  {
      //Must write to UCB0TXBUF
      switch (SlaveMode)
      {
          case (TX_DATA_MODE):
              UCB0TXBUF = TransmitBuffer[TransmitIndex++];
              TXByteCtr--;
              if (TXByteCtr == 0)
              {
                  //Done Transmitting MSG
                  SlaveMode = RX_REG_ADDRESS_MODE;
                  IE2 &= ~(UCB0TXIE);
                  IE2 |= UCB0RXIE;                          // Enable RX interrupt
                  I2C_Slave_TransactionDone(ReceiveRegAddr);
              }
              break;
          default:
              __no_operation();
              break;
      }

  }
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
    if (UCB0STAT & UCSTPIFG)                        //Stop or NACK Interrupt
    {
        UCB0STAT &=
            ~(UCSTTIFG + UCSTPIFG + UCNACKIFG);     //Clear START/STOP/NACK Flags
    }
    if (UCB0STAT & UCSTTIFG)
    {
        UCB0STAT &= ~(UCSTTIFG);                    //Clear START Flags
    }
}
