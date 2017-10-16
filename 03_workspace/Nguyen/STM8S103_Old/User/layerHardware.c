/**
  ******************************************************************************
  * SOURCE FILE OF STM8 BOARD HARDWARE LAYER
  * File        layerHardware.c
  * Author      HgN
  * Version     V1.0.0
  * Date        May   25th, 2017
  * Last update May   25th, 2017
  ******************************************************************************
  */


/* Include -------------------------------------------------------------------*/
#include "layerHardware.h"

/* Variables -----------------------------------------------------------------*/
uint32_t gTimerSecond = 0;

/* Functions -----------------------------------------------------------------*/

/* DEVICE INIT
 * Init all device, module, peripheral of the board
 * Watchdog timer enable
 */
void deviceInit()
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    timerInit(0xFFFF);
    timerIntEnable();
    //i2cInit();
    uartInit();
    //bh1750Init();
    //ds1307Init(17, 5, 27, 18, 40);
    shtInit();
    gpioPinMode(DEVICE_1_PORT, DEVICE_1_PIN, GPIO_OUTPUT);
    gpioPinMode(DEVICE_2_PORT, DEVICE_2_PIN, GPIO_OUTPUT);
    gpioPinMode(DEVICE_3_PORT, DEVICE_3_PIN, GPIO_OUTPUT);
    enableInterrupts();
}


/* DEVICE GET STATUS
 * Get the status of an output device
 * ==ARGS==
 * __pDevice: Name of the device
 * ==RETURN==
 * __The device status, 0 (0x00) if OFF, 100 (0x64) if ON
 */
uint8_t deviceGetStatus(uint8_t pDevice)
{
	uint8_t devicePin;
	uint16_t devicePort;
	switch(pDevice)
	{
		case DEVICE_ONOFF_1:
			devicePin = DEVICE_1_PIN;
		break;
		case DEVICE_ONOFF_2:
			devicePin = DEVICE_2_PIN;
		break;
		case DEVICE_ONOFF_3:
			devicePin = DEVICE_3_PIN;
		break;
		default:
			devicePin = DEVICE_NONE;
		break;
	}
	switch(pDevice)
	{
		case DEVICE_ONOFF_1:
			devicePort = DEVICE_1_PORT;
		break;
		case DEVICE_ONOFF_2:
			devicePort = DEVICE_2_PORT;
		break;
		case DEVICE_ONOFF_3:
			devicePort = DEVICE_3_PORT;
		break;
		default:
			devicePort = DEVICE_NONE;
		break;
	}
	if (gpioReadPin(devicePort, devicePin) == HIGH)
		return 0x64;
	else
		return 0x00;
} //TODO: Update for dim device


/* WATCHDOG INIT
 * Init the independence watchdog timer
 */
void watchdogInit()
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(0xFF);
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
    IWDG_Enable();
}


/* WATCHDOG REFRESH
 * Refresh the watchdog timer
 */
void watchdogRefresh()
{
	IWDG_ReloadCounter();
}


/* TIMER GET SECOND
 * Get the second value from timer2
 * ==RETURN==
 * __Return second value since timer run
 */
uint32_t timerGetSecond()
{
    return gTimerSecond;
}


/* TIMER 2 OVERFLOW INTERRUPT HANDLER
 * Use timer2 as an second counter
 */
#pragma vector = 15
__interrupt void timerInt()
{
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
    gTimerSecond++;
}
