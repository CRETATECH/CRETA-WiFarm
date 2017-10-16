/*******************************************************************************
  * @filename   : uart.c
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* REVISION HISTORY */
// March 17: Initialization.
// July 19: Update Project, clean file
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "uart.h"
#include "gpio.h"
#include <stdbool.h>
#include <stdlib.h>

/******************************************************************************/
/* LOCAL TYPEDEFS */
/******************************************************************************/

/******************************************************************************/
/* LOCAL DEFINES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC VARIABLES */
/******************************************************************************/
volatile uint8_t gBufferUART[UART_BUFFER_MAX]; // UART FIFO BUFFER
volatile uint8_t pBufferUARTRead = 0;
volatile uint8_t pBufferUARTWrite = 1;

/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief To init uart1
 * @param pBaudRate
 * @ret
 *      EXIT_SUCCESS
 *      EXIT_FAILURE
 */
int uartInit(uint32_t pBaudRate)
{
    //UART1_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
    UART1_Init(pBaudRate, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
        UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    UART1_ITConfig(UART1_IT_RXNE, ENABLE);
    UART1_Cmd(ENABLE);
    return EXIT_SUCCESS;
}

/**
 * @brief To read 1 byte from uart buffer
 * @param None
 * @ret   1 byte from uart buffer
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

/**
 * @brief To return uart buffer length
 * @param None
 * @ret   Length of uart buffer
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

/**
 * @brief To write data to uart line
 * @param *pData
 * @param pLength
 */
void uartWrite(uint8_t* pData, uint8_t pLength)
{
    while (pLength != 0) {
        uartWriteByte(*pData);
        pData++;
        pLength--;
    }
}

/**
 * @brief To write 1 byte to uart line
 * @param pByte
 */
void uartWriteByte(uint8_t pByte)
{
    while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
    UART1_SendData8(pByte);
}

/**
 * @brief UART RX interrupt handler
 */
#pragma vector = 20
__interrupt void uartRxIntHandler(void)
{
    UART1_ClearITPendingBit(UART1_IT_RXNE);
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

/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
