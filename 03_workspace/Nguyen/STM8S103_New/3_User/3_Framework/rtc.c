/*******************************************************************************
  * @filename   : rtc.c
  * @author     : HgN
  * @last update: July 20th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "rtc.h"
#include <stdbool.h>
#include <stdlib.h>
/******************************************************************************/
/* LOCAL TYPEDEFS */
/******************************************************************************/
typedef enum rtc_init
{
    RTC_INITIALIZED     = 1,
    RTC_UNINITIALIZED   = 0
} rtc_init_t;
/******************************************************************************/
/* LOCAL DEFINES */
/******************************************************************************/

/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/
void rtcDoTask(void);
void rtcClearTask(void);
/******************************************************************************/
/* PUBLIC VARIABLES */
/******************************************************************************/
rtc_init_t _rtc_status = RTC_UNINITIALIZED;

volatile uint32_t gTimerSecond = 0;
volatile uint32_t gTaskSecond = 0;
/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief   To initialize all module needed by RTC function
 * @param   None
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int rtcInit(void)
{
    // Init timer for second counter
    timerInit(0xFFFF);
    timerIntEnable();
    if(EXIT_SUCCESS != rtcGetRealTime())
    {
        _rtc_status = RTC_UNINITIALIZED;
        return EXIT_FAILURE;
    }
    rtcReadTask();
    _rtc_status = RTC_INITIALIZED;
    return EXIT_SUCCESS;
}
/**
 * @brief   To update real time IC
 * @details Send a request frame to get current time from server, then update
 *          real time IC
 * @param   None
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int rtcGetRealTime(void)
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    int vTimeOut = 0xFFFF;
    serialTX_t serialTxFrame;
    serialRX_t serialRxFrame;
    //<! Send request
    serialTxFrame.start = '*';
    serialTxFrame.stop = '#';
    serialTxFrame.func = FRAME_FUNC_RTC;
    serialSendFrame(&serialTxFrame);
    //<! Wait to get time
    while((serialGetFrame(&serialRxFrame) != EXIT_SUCCESS)&&(vTimeOut != 0))
    {
        vTimeOut--;
    }
    //<! Time out
    if (vTimeOut == 0)
        return EXIT_FAILURE;
    //<! Wrong frame
    if (serialRxFrame.func != FRAME_FUNC_TIME)
        return EXIT_FAILURE;
    //<! Update real time IC
    else
    {
        year = serialRxFrame.year;
        month = serialRxFrame.month;
        day = serialRxFrame.day;
        hour = serialRxFrame.hour;
        min = serialRxFrame.minute;
        ds1307SetTime(year, month, day, hour, min);
    }
    return EXIT_SUCCESS;
}
/**
 * @brief   To save task get from serial command to internal eeprom
 * @param   *serialRxFrame
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int rtcSaveTask(serialRX_t* serialRxFrame)
{
    if(EXIT_SUCCESS != serialCheckRxFrame(serialRxFrame))
        return EXIT_FAILURE;
    if(serialRxFrame->func != FRAME_FUNC_RTC)
        return EXIT_FAILURE;
    eepromWrite(EEPROM_START_ADDR, &(serialRxFrame->func), 1);
    eepromWrite(EEPROM_START_ADDR + 1, &(serialRxFrame->addr), 1);
    eepromWrite(EEPROM_START_ADDR + 2, &(serialRxFrame->hour), 1);
    eepromWrite(EEPROM_START_ADDR + 3, &(serialRxFrame->minute), 1);
    eepromWrite(EEPROM_START_ADDR + 4, &(serialRxFrame->dataH), 1);
    eepromWrite(EEPROM_START_ADDR + 5, &(serialRxFrame->dataL), 1);
    return EXIT_SUCCESS;
}
/**
 * @brief   To read saved tasked from eeprom
 * @param   None
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int rtcReadTask(void)
{
    if (_rtc_status != RTC_INITIALIZED) return EXIT_FAILURE;
    uint8_t vTask;
    uint8_t vTaskHour;
    uint8_t vTaskMin;
    uint8_t vCurHour;
    uint8_t vCurMin;
    uint32_t vTaskSec;
    uint32_t vCurSec;
    eepromRead(EEPROM_START_ADDR, &vTask, 1);
    if(vTask != FRAME_FUNC_RTC)
        return EXIT_FAILURE;
    //<! Get Task time
    eepromRead(EEPROM_START_ADDR + 2, &vTaskHour, 1);
    eepromRead(EEPROM_START_ADDR + 3, &vTaskMin, 1);
    //<! Get current time
    vCurHour = ds1307Read(DS1307_REG_HOUR);
    vCurMin = ds1307Read(DS1307_REG_MIN);
    //<! Compare and calculate time
    vTaskSec = ((uint32_t)vTaskHour*60*60)+(uint32_t)vTaskMin*60;
    vCurSec = ((uint32_t)vCurHour*60*60)+(uint32_t)vCurMin*60;
    if (vCurSec > vTaskSec)
        gTaskSecond = vCurSec - vTaskSec + gTimerSecond;
    else
        gTaskSecond = vCurSec - vTaskSec + 86400 + gTimerSecond;
    return EXIT_SUCCESS;
}
/**
 * @brief   To return second value since rtc init
 * @param   None
 * @retval  gTimerSecond
 */
uint32_t rtcGetTimerSecond(void)
{
    return gTimerSecond;
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/
/**
 * @brief To execute saved task when time reach
 * @param None
 */
void rtcDoTask(void)
{
    uint8_t vDevice;
    uint8_t vData;
    //<! Read device and value
    eepromRead(EEPROM_START_ADDR + 1, &vDevice, 1);
    eepromRead(EEPROM_START_ADDR + 5, &vData, 1);
    //<! Convert parameter
    switch(vDevice)
    {
    case FRAME_DEVICE_1:
        vDevice = DEVICE_1;
        break;
    case FRAME_DEVICE_2:
        vDevice = DEVICE_1;
        break;
    default:
        vDevice = DEVICE_NONE;
        break;
    }
    if(vData < 50)
        vData = DEVICE_OFF;
    else
        vData = DEVICE_ON;
    //<! Execute task
    if(vDevice != DEVICE_NONE)
        deviceControl(vDevice, vData);
}
/**
 * @brief Clear all task from eeprom
 * @param None
 */
void rtcClearTask(void)
{
    eepromClear();
}
/**
 * @brief Timer interrupt handler
 */
#pragma vector = 15
__interrupt void rtcSecondCounter(void)
{
    //<! Clear pending bit
    timerIntFlagClear();
    //<! Second counter
    gTimerSecond++;
    //<! No task
    if(gTaskSecond == 0)
        return;
    //<! There's task
    if(gTimerSecond == gTaskSecond)
    {
        rtcDoTask();
        rtcClearTask();
        gTaskSecond = 0;
    }
}
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
