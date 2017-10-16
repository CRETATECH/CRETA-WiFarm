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

typedef enum timer2_init
{
    TIMER_INITIALIZED   = 1,
    TIMER_UNINITIALIZED = 0
} timer2_init_t;
/******************************************************************************/
/* LOCAL DEFINES */
/******************************************************************************/
#define RTC_EEPROM_TASK1    EEPROM_START_ADDR
#define RTC_EEPROM_TASK2    RTC_EEPROM_TASK1 + 5
#define RTC_EEPROM_TASK3    RTC_EEPROM_TASK2 + 5
#define RTC_EEPROM_TASK4    RTC_EEPROM_TASK3 + 5
/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/
void rtcDoTask(void);
/******************************************************************************/
/* PUBLIC VARIABLES */
/******************************************************************************/
rtc_init_t _rtc_status = RTC_UNINITIALIZED;
timer2_init_t _timer2_status = TIMER_UNINITIALIZED;

volatile uint32_t gTimer100MS = 0;
volatile uint32_t gTimerSecond = 0;
volatile uint32_t gTaskSecond = 0;
volatile uint8_t gTaskNum = 0;
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
    uint8_t vTemp;
    //<! Run timer2
    rtcTimerRun();
    //<! Set real time
    if(EXIT_SUCCESS != ds1307Read(DS1307_REG_SEC, &vTemp))
    {
        _rtc_status = RTC_UNINITIALIZED;
        return EXIT_FAILURE;
    }
    else
        _rtc_status = RTC_INITIALIZED;
    rtcSetRunningTask();
    return EXIT_SUCCESS;
}
/**
 * @brief   To initialize second counter timer2
 * @param   None
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int rtcTimerRun(void)
{
    //<! Init timer2 for second counter
    timer2Init(0x186A);
    timer2IntEnable();
    _timer2_status = TIMER_INITIALIZED;
    return EXIT_SUCCESS;
}
/**
 * @brief   To update real time IC
 * @param   None
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int rtcSetRealTime(serial_frame_t *serialRxFrame)
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    //<! Wrong func
    if (serialRxFrame->func != SERIAL_FRAME_FUNC_SETTIME)
        return EXIT_FAILURE;
    //<! Update real time IC
    else
    {
        year = serialRxFrame->year;
        month = serialRxFrame->month;
        day = serialRxFrame->day;
        hour = serialRxFrame->hour;
        min = serialRxFrame->min;
    }
    if(EXIT_SUCCESS != ds1307SetTime(year, month, day, hour, min))
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
/**
 * @brief   To save task get from serial command to internal eeprom
 * @param   *serialRxFrame
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int rtcSaveTask(serial_frame_t* serialRxFrame)
{
    static int _task_num = 0;
    uint32_t vTaskAddr;
    switch(_task_num)
    {
    case 0: vTaskAddr = RTC_EEPROM_TASK1; break;
    case 1: vTaskAddr = RTC_EEPROM_TASK2; break;
    case 2: vTaskAddr = RTC_EEPROM_TASK3; break;
    case 3: vTaskAddr = RTC_EEPROM_TASK4; break;
    }
    if(serialRxFrame->func != SERIAL_FRAME_FUNC_RTC)
        return EXIT_FAILURE;
    eepromWrite(vTaskAddr, &(serialRxFrame->func), 1);
    eepromWrite(vTaskAddr + 1, &(serialRxFrame->addr), 1);
    eepromWrite(vTaskAddr + 2, &(serialRxFrame->hour), 1);
    eepromWrite(vTaskAddr + 3, &(serialRxFrame->min), 1);
    eepromWrite(vTaskAddr + 4, &(serialRxFrame->dataL), 1);

    _task_num++;
    if(_task_num == 4)
        _task_num = 0;
    return EXIT_SUCCESS;
}
/**
 * @brief   To read saved tasked from eeprom
 * @param   None
 * @retval  Seconds to task
 */
uint32_t rtcReadTask(uint8_t pTaskNum)
{
    uint32_t vTaskAddr = RTC_EEPROM_TASK1;
    switch(pTaskNum)
    {
    case 0: vTaskAddr = RTC_EEPROM_TASK1; break;
    case 1: vTaskAddr = RTC_EEPROM_TASK2; break;
    case 2: vTaskAddr = RTC_EEPROM_TASK3; break;
    case 3: vTaskAddr = RTC_EEPROM_TASK4; break;
    default: break;
    }
    uint8_t vTask;
    uint8_t vTaskHour;
    uint8_t vTaskMin;
    uint8_t vCurHour;
    uint8_t vCurMin;
    uint8_t vCurSecond;
    uint32_t vTaskSec;
    uint32_t vCurSec;
    //<! Check func
    eepromRead(vTaskAddr, &vTask, 1);
    if(vTask != SERIAL_FRAME_FUNC_RTC)
        return 0;
    //<! Get Task time
    eepromRead(vTaskAddr + 2, &vTaskHour, 1);
    eepromRead(vTaskAddr + 3, &vTaskMin, 1);
    //<! Get current time
    ds1307Read(DS1307_REG_HOUR, &vCurHour);
    ds1307Read(DS1307_REG_MIN, &vCurMin);
    ds1307Read(DS1307_REG_SEC, &vCurSecond);
    //<! Compare and calculate time
    vTaskSec = (((uint32_t)vTaskHour)*60*60)+((uint32_t)vTaskMin)*60;
    vCurSec = (((uint32_t)vCurHour)*60*60)+((uint32_t)vCurMin)*60+(uint32_t)vCurSecond;
    if (vCurSec >= vTaskSec)
        return (vCurSec - vTaskSec + 86400 + gTimerSecond);
    else
        return (vTaskSec - vCurSec + gTimerSecond);
}
/**
 * @brief   To set waiting task
 * @param   None
 * @retval
 */
void rtcSetRunningTask(void)
{
    if (_rtc_status != RTC_INITIALIZED)
    {
        gTaskSecond = 0;
        serialSendErr(SERIAL_FRAME_ERR_RTC);
        return;
    }
    uint32_t vTimeTemp = 0;
    uint32_t vTime = 0xFFFFFFFF;
    int i;
    for(i = 0; i<4; i++)
    {
        vTimeTemp = rtcReadTask(i);
        if((vTimeTemp != 0)&&(vTimeTemp < vTime))
        {
            vTime = vTimeTemp;
            gTaskNum = i;
        }
    }
    if(vTime != 0xFFFFFFFF)
        gTaskSecond = vTime;
    else
        gTaskSecond = 0;
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
/**
 * @brief   To return 100ms value since rtc init
 * @param   None
 * @retval  gTimerSecond
 */
uint32_t rtcGetTimer100MS(void)
{
    return gTimer100MS;
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
    uint32_t vTaskAddr;
    switch(gTaskNum)
    {
    case 0: vTaskAddr = RTC_EEPROM_TASK1; break;
    case 1: vTaskAddr = RTC_EEPROM_TASK2; break;
    case 2: vTaskAddr = RTC_EEPROM_TASK3; break;
    case 3: vTaskAddr = RTC_EEPROM_TASK4; break;
    default: break;
    }
    uint8_t vDevice;
    uint8_t vData;
    //<! Read device and value
    eepromRead(vTaskAddr + 1, &vDevice, 1);
    eepromRead(vTaskAddr + 4, &vData, 1);
    //<! Execute task
    if(IS_DEVICE_OK(vDevice))
    {
        deviceControl(vDevice, vData);
        serialSendDeviceStatus(vDevice, deviceRead(vDevice));
    }
}
/**
 * @brief Timer interrupt handler
 */
#pragma vector = 15
__interrupt void rtcSecondCounter(void)
{
    //<! Clear pending bit
    timer2IntFlagClear();
    //<! Miliseconds counter
    gTimer100MS++;
    //<! Second counter
    if((gTimer100MS % 10) == 0)
    gTimerSecond++;
    //<! No task
    if(gTaskSecond == 0)
        return;
    //<! There's task
    if(gTimerSecond == gTaskSecond)
    {
        rtcDoTask();
        rtcSetRunningTask();
    }
}
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
