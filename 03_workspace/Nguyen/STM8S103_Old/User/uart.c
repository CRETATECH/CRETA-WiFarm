/**
  ******************************************************************************
  * SOURCE FILE BASE MACHINE UART
  * File        uart.c 
  * Author      HgN
  * Version     V1.0.0
  * Date        March 18th, 2017    
  * Last update May   25th, 2017
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "uart.h"
#include "gpio.h"

/* Global variables ----------------------------------------------------------*/
volatile uint8_t gBufferUART[UART_BUFFER_MAX]; // UART FIFO BUFFER
volatile uint8_t pBufferUARTRead = 0;
volatile uint8_t pBufferUARTWrite = 1;

/* Functions -----------------------------------------------------------------*/

/* UART UART1_Init 
 * Tx+Rx Enable, Data 8 bis, Stop bits = 1, Parity = 0, No Sync
 * Receive Interrupt Enable
 */
uint8_t uartInit(void)
{
    UART1_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
    UART1_Init(BAUD_SPEED, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
        UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    UART1_ITConfig(UART1_IT_RXNE, ENABLE);
    UART1_Cmd(ENABLE);
    return SYS_OK;
}

/* UART READ
 * Read 1 byte from UART buffer
 * ==RETURN==
 * __Oldest byte from UART FIFO buffer
 */
uint8_t uartRead(void)
{
    uint8_t uartBufferReadTemp = pBufferUARTRead + 1;
    uint8_t uartReturnByte;
    if (uartBufferReadTemp == UART_BUFFER_MAX) {
        uartBufferReadTemp = 0;
    }
    if (uartBufferReadTemp != pBufferUARTWrite) {
        pBufferUARTRead = uartBufferReadTemp;
        uartReturnByte = gBufferUART[pBufferUARTRead];
    }
    return uartReturnByte;
}

/* UART Check buffer
 * ==RETURN==
 * __Current length of buffer.
 */
uint8_t uartAvailable(void)
{
    uint8_t uartBufferLength;
    if (pBufferUARTWrite > pBufferUARTRead) {
        uartBufferLength = pBufferUARTWrite - pBufferUARTRead - 1;
    } else {
        uartBufferLength = UART_BUFFER_MAX + pBufferUARTWrite - pBufferUARTRead - 1;
    }
    return uartBufferLength;
}

/* UART WRITE DATA
 * Send x byte to UART line
 * ==ARGS==
 * __*pData: data buffer
 * __pLength: how many byte
 */
void uartWrite(uint8_t* pData, uint8_t pLength)
{
    while (pLength != 0) {
        uartWriteByte(*pData);
        pData++;
        pLength--;
    }
}

/* UART WRITE BYTE
 * Send 1 byte to UART line
 * ==ARGS==
 * __pByte: byte to send
 */
void uartWriteByte(uint8_t pByte)
{
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    UART1_SendData8(pByte);
}

/* UART RX INTERRUPT HANDLER
 * Write recieve byte to UART buffer
 */
#pragma vector = 20
__interrupt void uartRxIntHandler(void)
{
    UART1_ClearITPendingBit(UART1_IT_RXNE_OR);
    if (pBufferUARTWrite != pBufferUARTRead) {
        gBufferUART[pBufferUARTWrite] = UART1_ReceiveData8();
        pBufferUARTWrite++;
        if (pBufferUARTWrite == UART_BUFFER_MAX) {
            pBufferUARTWrite = 0;
        }
    }
}

/*
#pragma vector = 19
__interrupt void uartTxIntHandler(void)
{
    while (gUARTWriteLength != 0)
    {
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
        UART1_SendData8(*gUARTWriteBuff);
        gUARTWriteBuff++;
        gUARTWriteLength--;
    }
    UART1_ITConfig(UART1_IT_TXE, DISABLE);
}
*/
