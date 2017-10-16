/*******************************************************************************
  * @filename   : serialProtocol.h
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

#ifndef SERIAL_H
#define SERIAL_H

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "stm8s.h"
#include "defineHardware.h"

/******************************************************************************/
/* PUBLIC TYPEDEFS */
/******************************************************************************/
typedef struct serialTX
{
    uint8_t start;
    uint8_t func;
    uint8_t addr;
    uint8_t stat;
    uint16_t geoX;
    uint16_t geoY;
    uint16_t geoZ;
    uint8_t dataH;
    uint8_t dataL;
    uint8_t stop;
} serialTX_t;

typedef struct serialRX
{
    uint8_t start;
    uint8_t func;
    uint8_t addr;
    uint8_t stat;
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t dataH;
    uint8_t dataL;
    uint8_t stop;
}serialRX_t;

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/
#define SERIAL_FRAME_RX_LENGTH 12
#define SERIAL_FRAME_TX_LENGTH 13
#define SERIAL_FRAME_START_ELEMENT   '*'
#define SERIAL_FRAME_STOP_ELEMENT    '#'

#define FRAME_FUNC_DIM              1
#define FRAME_FUNC_RTC              2
#define FRAME_FUNC_TIME             3
#define FRAME_FUNC_SENSOR           5
#define FRAME_FUNC_STATUS           11

#define FRAME_DEVICE_1              1
#define FRAME_DEVICE_2              2
#define FRAME_DEVICE_3              3
#define FRAME_DEVICE_LUX            4
#define FRAME_DEVICE_TEMP           5
#define FRAME_DEVICE_HUMID          6

#define FRAME_STAT_ON               1
#define FRAME_STAT_OFF              0

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES */
/******************************************************************************/
int serialGetFrame(serialRX_t* serialRxFrame);
int serialSendFrame(serialTX_t *serialTxFrame);
void serialClearTxFrame(serialTX_t *serialTxFrame);
void serialClearRxFrame(serialRX_t *serialRxFrame);
int serialCheckTxFrame(serialTX_t *serialTxFrame);
int serialCheckRxFrame(serialRX_t* serialRxFrame);

#endif
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/