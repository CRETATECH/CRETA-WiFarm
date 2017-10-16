#include "board.h"

uint32_t timerSecond = 0;

uint8_t uartWriteBuff[20];
uint8_t*  writeStart    = &uartWriteBuff[0];
uint8_t*  writeFunction = &uartWriteBuff[1];
uint8_t*  writeAddress  = &uartWriteBuff[2];
uint8_t*  writeStatus   = &uartWriteBuff[3];
uint16_t* writeGeodeX   = (uint16_t*)(&uartWriteBuff[4]);
uint16_t* writeGeodeY   = (uint16_t*)(&uartWriteBuff[6]);
uint16_t* writeGeodeZ   = (uint16_t*)(&uartWriteBuff[8]);
uint8_t*  writeDataHigh = &uartWriteBuff[10];
uint8_t*  writeDataLow  = &uartWriteBuff[11];
uint8_t*  writeEnd      = &uartWriteBuff[12];

// Cmd Analyze
uint8_t uartCmdBuff[20];
uint8_t* readStart      = &uartCmdBuff[0];
uint8_t* readFunction   = &uartCmdBuff[1];
uint8_t* readAddress    = &uartCmdBuff[2];
uint8_t* readStatus     = &uartCmdBuff[3];
uint8_t* readYear       = &uartCmdBuff[4];
uint8_t* readMonth      = &uartCmdBuff[5];
uint8_t* readDay        = &uartCmdBuff[6];
uint8_t* readHour       = &uartCmdBuff[7];
uint8_t* readMinute     = &uartCmdBuff[8];
uint8_t* readDataHigh   = &uartCmdBuff[9];
uint8_t* readDataLow    = &uartCmdBuff[10];
uint8_t* readEnd        = &uartCmdBuff[11];

void cmdDim();
void cmdRTC();
void cmdSetTime();

uint32_t timerGetSecond()
{
    return timerSecond;
}

void clkInit()
{
    CLK_DeInit();
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    CLK_AdjustHSICalibrationValue(CLK_HSITRIMVALUE_0);
}

uint8_t cmdGet()
{
    uint8_t* uartCmdBuffPointer = readStart;
    bool uartCmdGetFinished = false;
    if (uartAvailable())
    {
        if(uartRead() == '*')
        {
            *readStart = '*';
            uartCmdBuffPointer++;
            uint8_t uartCmdGetWaiting = timerSecond;
            while(uartCmdGetFinished != true)
            {
                if(uartAvailable())
                {
                    *uartCmdBuffPointer = uartRead();
                    if (*uartCmdBuffPointer == '#')
                        uartCmdGetFinished = true;
                    else uartCmdBuffPointer++;
                }
                if((timerSecond-uartCmdGetWaiting)>2)
                    uartCmdGetFinished = true;
            }
        }
        else *readStart = 0x00;
    }
    else return SYS_ERR;
    if ((*readEnd == '#')&(*readStart == '*'))
    {
        *readEnd = 0;
        *readStart = 0;
        return SYS_OK;
    }
    else
    {
        *readEnd = 0;
        *readStart = 0;
        return SYS_ERR;
    }
}

void cmdDoRequest()
{
    switch(*readFunction)
    {
    case FUNCTION_DIM:
        cmdDim();
        break;
    case FUNCTION_RTC:
        cmdRTC();
        break;
    case FUNCTION_TIME:
        cmdSetTime();
        break;
    }
}

void cmdDim()
{
    uint8_t deviceValue;
    if (*readDataLow > 50)
        deviceValue = HIGH;
    else deviceValue = LOW;
    switch(*readAddress)
    {
    case DEVICE_ONOFF_1:
        gpioWritePin(GPIO_PORTD, PIN_4, deviceValue);
        break;
    case DEVICE_ONOFF_2:
        gpioWritePin(GPIO_PORTA, PIN_1, deviceValue);
        break;
    case DEVICE_ONOFF_3:
        gpioWritePin(GPIO_PORTA, PIN_2, deviceValue);
        break;
    case DEVICE_DIM_1:
        pwmInit(*readDataLow);
        break;
    default:
        break;
    }
}

void cmdRTC()
{
}

void cmdSetTime()
{
}

void cmdSensorReturn(uint8_t deviceAddress, uint16_t deviceData)
{
    *writeStart    = '*';
    *writeFunction = FUNCTION_DATA;
    *writeAddress  = deviceAddress;
    *writeStatus   = STATUS_OK;
    *writeDataHigh = ((uint8_t)(deviceData>>8));
    *writeDataLow  = ((uint8_t)deviceData);
    *writeEnd      = '#';
    uartWrite(writeStart, 13);
}

#pragma vector = 15
__interrupt void timerInt()
{
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE );
    timerSecond++;
}