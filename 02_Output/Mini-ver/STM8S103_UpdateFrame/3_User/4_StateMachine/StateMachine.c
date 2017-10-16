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
int gButton = BUTTON_NONE;
uint32_t gClock = 0;

serial_frame_t gSerialRxFrame;
serial_frame_t gSerialTxFrame;

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
    if(gSerialRxFrame.func == SERIAL_FRAME_FUNC_DEVICE)
        gState = STATE_DEVICE;
    //<! Execute RTC command
    else if(gSerialRxFrame.func == SERIAL_FRAME_FUNC_RTC)
        gState = STATE_RTC;
    //<! Execute data command
    else if(gSerialRxFrame.func == SERIAL_FRAME_FUNC_DATA)
        gState = STATE_SENSOR;
    //<! Get a command
    else if(uartAvailable() != 0)
        gState = STATE_CMD;
#ifdef BUTTON
    //<! Check button 1
    else if(GPIO_LOW == buttonCheck(BUTTON_1))
    {
        gButton = BUTTON_1;
        gState = STATE_DEVICE;
    }
    //<! Check button 2
    else if(GPIO_LOW == buttonCheck(BUTTON_2))
    {
        gButton = BUTTON_2;
        gState = STATE_DEVICE;
    }
#endif
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
#ifdef RTC
        case STATE_RTC:
            stateRTC();
        break;
#endif
		default:
		break;
	}
}

/**
 * @brief   To init all device
 * @param   None
 */
void stateInit(void)
{
    //<! Clock Init
    // CLK_DeInit();
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    // CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    CLK_AdjustHSICalibrationValue(CLK_HSITRIMVALUE_0);
    // gClock = CLK_GetClockFreq();
    //<! Internal eeprom initialize
    eepromInit();
    //<! I2C initialize
    i2cInit(I2C_CLOCK_STD, 0x23);
    //<! Device initialize
    deviceInit();
    //<! SHT initialize
    shtInit();
    //<! UART initialize
    uartInit(115200);
    //<! Run timer for RTC and other func
    rtcInit();
    //<! Global interrupt enable
    enableInterrupts();
    //<! Inform reset
    serialSendErr(SERIAL_FRAME_ERR_RST);
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
    //<! Check commamd frame
    if(EXIT_SUCCESS == serialGetFrame(&gSerialRxFrame))
    {
        //<! Answer device status request
        if(gSerialRxFrame.func == SERIAL_FRAME_FUNC_UPDATE)
        {
            serialSendDeviceStatus(SERIAL_FRAME_DEVICE_1, (uint16_t)(deviceRead(DEVICE_1)));
            serialSendDeviceStatus(SERIAL_FRAME_DEVICE_2, (uint16_t)(deviceRead(DEVICE_2)));
        }
        //<! Function set time
        else if(gSerialRxFrame.func == SERIAL_FRAME_FUNC_SETTIME)
        {
            if(EXIT_SUCCESS != rtcSetRealTime(&gSerialRxFrame))
            {
                //<! RTC Fail, send Err frame
                serialSendErr(SERIAL_FRAME_ERR_RTC);
            }
            else
            {
                //<! Echo frame
                serialSendFrame(&gSerialRxFrame);
                rtcSetRunningTask();
            }
        }
        else
        {
            //<! Do nothing but keep Rx frame for other state.
            return;
        }
    }
    //<! Clear all frame
    serialClearFrame(&gSerialRxFrame);
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
#ifdef SHT
    if(EXIT_SUCCESS == shtReadHumid(&fhumid))
    {
        humid = (uint16_t)(fhumid*100);
        serialSendDeviceStatus(SERIAL_FRAME_DEVICE_HUMID, humid);
    }
    else
        serialSendErr(SERIAL_FRAME_ERR_SENSOR);
#endif
    //<! Clear all frame
    serialClearFrame(&gSerialRxFrame);
}
/**
 * @brief   To control device and send device status.
 * @details This state is called by button pressed or serial device command
 * @param   None
 */
void stateDevice(void)
{
    uint8_t vDevice = 0;
    uint8_t vData = 0;
#ifdef BUTTON
    //<! If button pressed
    if(gButton != BUTTON_NONE)
    {
        // Set device
        if(gButton == BUTTON_1)
            vDevice = DEVICE_1;
        else
            vDevice = DEVICE_2;
        // Toggle device
        if(DEVICE_ON == deviceRead(vDevice))
            vData = DEVICE_OFF;
        else
            vData = DEVICE_ON;
        // Reset button flag
        gButton = BUTTON_NONE;
    }
#endif
    //<! If serial command
    if (gSerialRxFrame.func == SERIAL_FRAME_FUNC_DEVICE)
    {
        vDevice = gSerialRxFrame.addr;
        vData = gSerialRxFrame.dataL;
        serialClearFrame(&gSerialRxFrame);
    }

    if((!IS_FRAME_DATA_OK(vData))||(!IS_FRAME_DEV_OK(vDevice)))
    {
        serialSendErr(SERIAL_FRAME_ERR_DATA);
    }
    else
    {
        // Control device
        deviceControl(vDevice, vData);
        // Send respond frame
        serialSendDeviceStatus(vDevice, deviceRead(vDevice));
    }
}
/**
 * @brief   To do RTC command
 * @details This state is called by serial RTC command. When get the command,
 *          tasked and time is saved to internal eeprom.
 * @param   None
 */
void stateRTC(void)
{
    //<! Echo frame
    serialSendFrame(&gSerialRxFrame);
    rtcSaveTask(&gSerialRxFrame);
    rtcSetRunningTask();
    //<! Clear Rx Frame after done
    serialClearFrame(&gSerialRxFrame);
}
/**
 * @brief To do nothing
 * @param None
 */
void stateIdle(void)
{
    //<! Just clear everything
    // serialClearFrame(&gSerialTxFrame);
    // serialClearFrame(&gSerialRxFrame);
}
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
