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
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int serialGetFrame(serial_frame_t* serialRxFrame)
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
    if(EXIT_SUCCESS != serialCheckFrame(serialRxFrame))
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
/**
 * @brief To send serial frame
 * @param serialTxFrame
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int serialSendFrame(serial_frame_t *serialTxFrame)
{
    uint8_t *pBuffer;
    //<! Set start stop element
    serialTxFrame->start = SERIAL_FRAME_START_ELEMENT;
    serialTxFrame->stop = SERIAL_FRAME_STOP_ELEMENT;
    //<! Set pointer to struct
    pBuffer = &(serialTxFrame->start);
    //<! Send frame via uart
    uartWrite(pBuffer, SERIAL_FRAME_LENGTH);
    return EXIT_SUCCESS;
}
/**
 * @brief   To clear Tx frame
 * @param   *serialTxFrame
 */
void serialClearFrame(serial_frame_t *serialFrame)
{
    serialFrame->start  = 0;
    serialFrame->stop   = 0;
    serialFrame->func   = 0;
    serialFrame->addr   = 0;
    serialFrame->year   = 0;
    serialFrame->month  = 0;
    serialFrame->day    = 0;
    serialFrame->hour   = 0;
    serialFrame->min    = 0;
    serialFrame->dataH  = 0;
    serialFrame->dataL  = 0;
}
/**
 * @brief   To check if Tx frame is right
 * @param   *serialTxFrame
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int serialCheckFrame(serial_frame_t *serialFrame)
{

    //<! Check start stop element
    if((serialFrame->start != SERIAL_FRAME_START_ELEMENT) || \
        (serialFrame->stop != SERIAL_FRAME_STOP_ELEMENT))
    {
        //<! Send respond wrong frame
        serialSendErr(SERIAL_FRAME_ERR_FRAME);
        return EXIT_FAILURE;
    }
    //<! Check function
    else if(!IS_FRAME_FUNC_OK(serialFrame->func))
    {
        //<! Send respond wrong func
        serialSendErr(SERIAL_FRAME_ERR_FUNC);
        return EXIT_FAILURE;
    }
    else
    {
        //<! Send respond OK
        //serialSendRespond();
        return EXIT_SUCCESS;
    }
}
/**
 * @brief   To send error frame
 * @param   pErr
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int serialSendErr(uint8_t pErr)
{
    //<! Send respond frame
    serial_frame_t serialResFrame;
    serialClearFrame(&serialResFrame);
    serialResFrame.func = SERIAL_FRAME_FUNC_ERR;
    serialResFrame.dataL = pErr;
    serialSendFrame(&serialResFrame);
    return EXIT_SUCCESS;
}
/**
 * @brief   To send respond frame
 * @param   None
 */
/*
void serialSendRespond(void)
{
    serial_frame_t serialResFrame;
    serialClearFrame(&serialResFrame);
    serialResFrame.func = SERIAL_FRAME_FUNC_RES;
    serialSendFrame(&serialResFrame);
}
*/
/**
 * @brief   To send device status
 * @param   pAddr
 * @param   pValue
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int serialSendDeviceStatus(uint8_t pAddr, uint16_t pValue)
{
    serial_frame_t serialDevFrame;
    serialClearFrame(&serialDevFrame);
    serialDevFrame.func = SERIAL_FRAME_FUNC_DATA;
    serialDevFrame.addr = pAddr;
    serialDevFrame.dataH = (uint8_t)(pValue>>8);
    serialDevFrame.dataL = (uint8_t)pValue;
    serialSendFrame(&serialDevFrame);
    return EXIT_SUCCESS;
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
