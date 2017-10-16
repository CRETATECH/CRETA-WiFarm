/*******************************************************************************
  * @filename   : device.c
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "device.h"
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
 * @brief To init all device ON/OFF
 * @param None
 */
void deviceInit(void)
{
    gpioPinMode(DEVICE_1_PORT, DEVICE_1_PIN, GPIO_OUTPUT);
    gpioPinMode(DEVICE_2_PORT, DEVICE_2_PIN, GPIO_OUTPUT);
    gpioPinMode(DEVICE_3_PORT, DEVICE_3_PIN, GPIO_OUTPUT);
    gpioWritePin(DEVICE_1_PORT, DEVICE_1_PIN, GPIO_LOW);
    gpioWritePin(DEVICE_2_PORT, DEVICE_2_PIN, GPIO_LOW);
    //<! Turn err led off
    gpioWritePin(DEVICE_3_PORT, DEVICE_3_PIN, GPIO_HIGH);
}
/**
 * @brief To toggle device ON/OFF
 * @param pDevice
 */
void deviceToggle(uint8_t pDevice)
{
    if(GPIO_LOW == deviceRead(pDevice))
        deviceControl(pDevice, DEVICE_ON);
    else
        deviceControl(pDevice, DEVICE_OFF);
}
/**
 * @brief To control device ON/OFF
 * @param pDevice
 * @param pValue
 */
void deviceControl(uint8_t pDevice, uint8_t pValue)
{
    uint8_t devicePin;
	uint16_t devicePort;
    uint8_t deviceValue;
    // Get device pin
	switch(pDevice)
	{
		case DEVICE_1:
			devicePin = DEVICE_1_PIN;
		break;
		case DEVICE_2:
			devicePin = DEVICE_2_PIN;
		break;
		case DEVICE_3:
			devicePin = DEVICE_3_PIN;
		break;
		default:
			devicePin = DEVICE_NONE;
		break;
	}
    // Get device port
	switch(pDevice)
	{
		case DEVICE_1:
			devicePort = DEVICE_1_PORT;
		break;
		case DEVICE_2:
			devicePort = DEVICE_2_PORT;
		break;
		case DEVICE_3:
			devicePort = DEVICE_3_PORT;
		break;
		default:
			devicePort = DEVICE_NONE;
		break;
	}
    // Get device value
    if (DEVICE_ON == pValue)
        deviceValue = GPIO_HIGH;
    else
        deviceValue = GPIO_LOW;
    // Control the device
    gpioWritePin(devicePort, devicePin, deviceValue);
}
/**
 * @brief To read device value
 * @param pDevice
 * @ret   device value (DEVICE_ON/OFF)
 */
uint8_t deviceRead(uint8_t pDevice)
{
    uint8_t devicePin;
	uint16_t devicePort;
    // Get device pin
	switch(pDevice)
	{
		case DEVICE_1:
			devicePin = DEVICE_1_PIN;
		break;
		case DEVICE_2:
			devicePin = DEVICE_2_PIN;
		break;
		case DEVICE_3:
			devicePin = DEVICE_3_PIN;
		break;
		default:
			devicePin = DEVICE_NONE;
		break;
	}
    // Get device port
	switch(pDevice)
	{
		case DEVICE_1:
			devicePort = DEVICE_1_PORT;
		break;
		case DEVICE_2:
			devicePort = DEVICE_2_PORT;
		break;
		case DEVICE_3:
			devicePort = DEVICE_3_PORT;
		break;
		default:
			devicePort = DEVICE_NONE;
		break;
	}
    // Read device value
    if(GPIO_HIGH == gpioReadPin(devicePort, devicePin))
        return DEVICE_ON;
    else
        return DEVICE_OFF;
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
