/*******************************************************************************
  * @filename   : StateMachine.c
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "StateMachine.h"
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
int gState = STATE_IDLE;

serialRX_t gSerialRxFrame;
serialTX_t gSerialTxFrame;

/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/
void stateCmd(void);
void stateDevice(void);
void stateRTC(void);
void stateSensor(void);
void stateIdle(void);

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief   To update gState
 * @details State change depend on serial command, timer, etc.
 * @param   None
 */
void stateUpdate(void)
{
    static uint32_t vSensorTime = 0;
    //<! Execute device control command
    if(gSerialRxFrame.func == FRAME_FUNC_DIM)
        gState = STATE_DEVICE;
    //<! Execute RTC command
    else if(gSerialRxFrame.func == FRAME_FUNC_RTC)
        gState = STATE_RTC;
    //<! Get a command
    else if (uartAvailable() != 0)
        gState = STATE_CMD;
    //<! Periodic sensor data send
    else if ((rtcGetTimerSecond() - vSensorTime) > SENSOR_GAP)
    {
        gState = STATE_SENSOR;
        vSensorTime = rtcGetTimerSecond();
    }
    //<! Nothing to do
    else
        gState = STATE_IDLE;
}

/**
 * @brief   To change state
 * @param   None
 */
void stateMachine(void)
{
	switch(gState)
	{
		case STATE_CMD:
			stateCmd();
		break;
		case STATE_IDLE:
			stateIdle();
		break;
        case STATE_DEVICE:
            stateDevice();
        break;
        case STATE_SENSOR:
            stateSensor();
        break;
        case STATE_RTC:
            stateRTC();
        break;
		default:
		break;
	}
}

/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/
/**
 * @brief   State serial command
 * @details Machine will alway go to this state when uart Rx available. It will
 *          get the frame, check if it correct and update gSerialRxFrame.
 * @param   None
 */
void stateCmd(void)
{
    //<! Get command frame
    if(EXIT_SUCCESS != serialGetFrame(&gSerialRxFrame))
    {
        serialClearRxFrame(&gSerialRxFrame);
        return;
    }
    //<! Answer device status request
    if(gSerialRxFrame.func == FRAME_FUNC_STATUS)
    {
        //<! Prepair Tx frame
        gSerialTxFrame.start = SERIAL_FRAME_START_ELEMENT;
        gSerialTxFrame.stop  = SERIAL_FRAME_STOP_ELEMENT;
        gSerialTxFrame.func = FRAME_FUNC_STATUS;
        gSerialTxFrame.stat = FRAME_STAT_ON;
        //<! Send device 1 status
        gSerialTxFrame.addr = FRAME_DEVICE_1;
        gSerialTxFrame.dataL = deviceRead(DEVICE_1);
        serialSendFrame(&gSerialTxFrame);
        //<! Send device 2 status
        gSerialTxFrame.addr = FRAME_DEVICE_2;
        gSerialTxFrame.dataL = deviceRead(DEVICE_2);
        serialSendFrame(&gSerialTxFrame);
        //<! Clear all frame
        serialClearTxFrame(&gSerialTxFrame);
        serialClearRxFrame(&gSerialRxFrame);
    }
}
/**
 * @brief   To send sensor data.
 * @detail  This state is called after every SENSOR_GAP or by serial command.
 *          Sensor data multiplied by 100. Example: humid = 84.7568, send 8475.
 * @param   None
 */
void stateSensor(void)
{
    float fhumid;
    uint16_t humid;
    //<! Read and send humid data
    if(EXIT_SUCCESS == shtReadHumid(&fhumid))
    {
        humid = (uint16_t)(fhumid*100);
        gSerialTxFrame.start = SERIAL_FRAME_START_ELEMENT;
        gSerialTxFrame.stop  = SERIAL_FRAME_STOP_ELEMENT;
        gSerialTxFrame.func = FRAME_FUNC_SENSOR;
        gSerialTxFrame.addr = FRAME_DEVICE_HUMID;
        gSerialTxFrame.stat = FRAME_STAT_OFF;
        gSerialTxFrame.dataH = (uint8_t)(humid>>8);
        gSerialTxFrame.dataL = (uint8_t)humid;
        serialSendFrame(&gSerialTxFrame);
    }
    //<! Clear all frame
    serialClearTxFrame(&gSerialTxFrame);
    serialClearRxFrame(&gSerialRxFrame);
}
/**
 * @brief   To control device and send device status.
 * @details This state is called by button pressed or serial device command
 * @param   None
 */
void stateDevice(void)
{
    uint8_t vDevice;
    uint8_t vData;
    switch(gSerialRxFrame.addr)
    {
    case FRAME_DEVICE_1:
        vDevice = DEVICE_1;
        break;
    case FRAME_DEVICE_2:
        vDevice = DEVICE_2;
        break;
    default:
        vDevice = DEVICE_NONE;
        break;
    }
    if(gSerialRxFrame.dataL < 50)
        vData = DEVICE_OFF;
    else
        vData = DEVICE_ON;
    if(vDevice != DEVICE_NONE)
    {
        deviceControl(vDevice, vData);
        gSerialTxFrame.start = '*';
        gSerialTxFrame.stop = '#';
        gSerialTxFrame.func = FRAME_FUNC_STATUS;
        gSerialTxFrame.addr = gSerialRxFrame.addr;
        gSerialTxFrame.stat = FRAME_STAT_ON;
        gSerialTxFrame.dataL = deviceRead(vDevice);
        serialSendFrame(&gSerialTxFrame);
    }
    //<! Clear frame after done
    serialClearTxFrame(&gSerialTxFrame);
    serialClearRxFrame(&gSerialRxFrame);
}
/**
 * @brief   To do RTC command
 * @details This state is called by serial RTC command. When get the command,
 *          tasked and time is saved to internal eeprom.
 * @param   None
 */
void stateRTC(void)
{
    rtcSaveTask(&gSerialRxFrame);
    rtcReadTask();
    //<! Clear Rx Frame after done
    serialClearRxFrame(&gSerialRxFrame);
}
/**
 * @brief To do nothing
 * @param None
 */
void stateIdle(void)
{
    //<! Just clear everything
    serialClearTxFrame(&gSerialTxFrame);
    serialClearRxFrame(&gSerialRxFrame);
}
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
