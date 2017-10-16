/*******************************************************************************
  * @filename   : serialProtocol.c
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "serialProtocol.h"
#include "uart.h"
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

/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief   get serial command frame from uart buffer
 * @param   serialRxFrame
 */
int serialGetFrame(serialRX_t* serialRxFrame)
{
    int vTimeOut = 0xFFFF;
    bool flagDone = false;
    uint8_t *pBuffer;
    pBuffer = &(serialRxFrame->start);
    //<! Check start byte
    if(0 != uartAvailable())
    {
        *pBuffer = uartRead();
        if(serialRxFrame->start != SERIAL_FRAME_START_ELEMENT)
            return EXIT_FAILURE;
        else
            pBuffer++;
    }
    else
        return EXIT_FAILURE;
    //<! Get the frame
    while (false == flagDone)
    {
        if(0 != uartAvailable())
        {
            *pBuffer = uartRead();
            pBuffer++;
        }

        //<! Get all frame length
        if (pBuffer > &(serialRxFrame->stop))
            flagDone = true;
        //<! Time out
        else if (0 == vTimeOut)
            return EXIT_FAILURE;
        else
            vTimeOut--;
    }
    //<! Check frame
    if(EXIT_SUCCESS != serialCheckRxFrame(serialRxFrame))
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
/**
 * @brief To send serial frame
 * @param serialTxFrame
 */
int serialSendFrame(serialTX_t *serialTxFrame)
{
    if(EXIT_SUCCESS != serialCheckTxFrame(serialTxFrame))
        return EXIT_FAILURE;
    uint8_t *pBuffer;
    pBuffer = &(serialTxFrame->start);
    uartWrite(pBuffer, SERIAL_FRAME_TX_LENGTH);
    return EXIT_SUCCESS;
}
/**
 * @brief   To clear Tx frame
 * @param   *serialTxFrame
 */
void serialClearTxFrame(serialTX_t *serialTxFrame)
{
    serialTxFrame->start = 0;
    serialTxFrame->stop = 0;
    serialTxFrame->func = 0;
    serialTxFrame->addr = 0;
    serialTxFrame->stat = 0;
    serialTxFrame->geoX = 0;
    serialTxFrame->geoY = 0;
    serialTxFrame->geoZ = 0;
    serialTxFrame->dataH = 0;
    serialTxFrame->dataL = 0;
}
/**
 * @brief   To clear Rx frame
 * @param   *serialRxFrame
 */
void serialClearRxFrame(serialRX_t *serialRxFrame)
{
    serialRxFrame->start = 0;
    serialRxFrame->stop = 0;
    serialRxFrame->func = 0;
    serialRxFrame->addr = 0;
    serialRxFrame->stat = 0;
    serialRxFrame->year = 0;
    serialRxFrame->month = 0;
    serialRxFrame->day = 0;
    serialRxFrame->hour = 0;
    serialRxFrame->minute = 0;
    serialRxFrame->dataH = 0;
    serialRxFrame->dataL = 0;
}
/**
 * @brief   To check if Tx frame is right
 * @param   *serialTxFrame
 */
int serialCheckTxFrame(serialTX_t *serialTxFrame)
{
    if(serialTxFrame->start != '*')
        return EXIT_FAILURE;
    else if (serialTxFrame->stop != '#')
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
/**
 * @brief   To check if Rx frame is right
 * @param   *serialRxFrame
 */
int serialCheckRxFrame(serialRX_t* serialRxFrame)
{
    if(serialRxFrame->start != '*')
        return EXIT_FAILURE;
    else if (serialRxFrame->stop != '#')
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}

/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
