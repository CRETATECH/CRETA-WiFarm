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
    uint8_t start[2];
    uint8_t type;
    uint8_t func;
    uint8_t length;
    uint8_t id;
    uint8_t crc[2];
    uint8_t data[10];
} serial_frame_t;

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/
#define SERIAL_FRAME_LENGTH                 11
#define SERIAL_FRAME_START_ELEMENT_1        0x27
#define SERIAL_FRAME_START_ELEMENT_2        0x07
#define SERIAL_FRAME_STOP_ELEMENT           '#'

#define SERIAL_FRAME_FUNC_DATA              0x01
#define SERIAL_FRAME_FUNC_DEVICE            0x02
#define SERIAL_FRAME_FUNC_RTC               0x03
#define SERIAL_FRAME_FUNC_SETTIME           0x05
#define SERIAL_FRAME_FUNC_UPDATE            0x06
#define SERIAL_FRAME_FUNC_ERR               0xFE
#define SERIAL_FRAME_FUNC_RES               0xEF

#define IS_FRAME_FUNC_OK(func)  ((func == SERIAL_FRAME_FUNC_DATA)   || \
                                (func == SERIAL_FRAME_FUNC_DEVICE)  || \
                                (func == SERIAL_FRAME_FUNC_RTC)     || \
                                (func == SERIAL_FRAME_FUNC_SETTIME) || \
                                (func == SERIAL_FRAME_FUNC_UPDATE))

#define SERIAL_FRAME_DEVICE_1               DEVICE_1
#define SERIAL_FRAME_DEVICE_2               DEVICE_2
#define SERIAL_FRAME_DEVICE_3               DEVICE_3
#define SERIAL_FRAME_DEVICE_LUX             0x04
#define SERIAL_FRAME_DEVICE_TEMP            0x05
#define SERIAL_FRAME_DEVICE_HUMID           0x06

#define IS_FRAME_ADDR_OK(dev)    ((dev == SERIAL_FRAME_DEVICE_1)     || \
                                 (dev == SERIAL_FRAME_DEVICE_2)     || \
                                 (dev == SERIAL_FRAME_DEVICE_3)     || \
                                 (dev == SERIAL_FRAME_DEVICE_LUX)   || \
                                 (dev == SERIAL_FRAME_DEVICE_TEMP)  || \
                                 (dev == SERIAL_FRAME_DEVICE_HUMID))

#define IS_FRAME_DEV_OK(dev)     ((dev == SERIAL_FRAME_DEVICE_1)     || \
                                 (dev == SERIAL_FRAME_DEVICE_2)     || \
                                 (dev == SERIAL_FRAME_DEVICE_3))


#define SERIAL_FRAME_ERR_NONE               0x10
#define SERIAL_FRAME_ERR_FRAME              0x11
#define SERIAL_FRAME_ERR_FUNC               0x12
#define SERIAL_FRAME_ERR_DATA               0x13
#define SERIAL_FRAME_ERR_RTC                0x14
#define SERIAL_FRAME_ERR_SENSOR             0x15
#define SERIAL_FRAME_ERR_RST                0x16

#define IS_FRAME_ERR_OK(err)    ((err == SERIAL_FRAME_ERR_FRAME)    || \
                                 (err == SERIAL_FRAME_ERR_FUNC)     || \
                                 (err == SERIAL_FRAME_ERR_DATA)     || \
                                 (err == SERIAL_FRAME_ERR_RTC)      || \
                                 (err == SERIAL_FRAME_ERR_SENSOR)   || \
                                 (err == SERIAL_FRAME_ERR_RST))

#define SERIAL_FRAME_DATA_ON                DEVICE_ON
#define SERIAL_FRAME_DATA_OFF               DEVICE_OFF

#define IS_FRAME_DATA_OK(data)  ((data == SERIAL_FRAME_DATA_ON) || \
                                 (data == SERIAL_FRAME_DATA_OFF))

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES */
/******************************************************************************/
int serialGetFrame (serial_frame_t* serialRxFrame);
int serialSendFrame(serial_frame_t *serialFrame);
void serialClearFrame(serial_frame_t *serialFrame);
int serialCheckFrame(serial_frame_t *serialFrame);
int serialSendDeviceStatus(uint8_t pAddr, uint16_t pValue);
int serialSendErr(uint8_t pErr);
//void serialSendRespond(void);

#endif
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/