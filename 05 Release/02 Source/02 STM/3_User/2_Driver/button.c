/*******************************************************************************
  * @filename   : hutton.c
  * @author     : HgN
  * @last update: July 20th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "button.h"
#include "gpio.h"
#include "rtc.h"
#include <stdbool.h>
#include <stdlib.h>
/******************************************************************************/
/* LOCAL TYPEDEFS */
/******************************************************************************/

/******************************************************************************/
/* LOCAL DEFINES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
void buttonInit(void)
{
    gpioPinMode(BUTTON_1_PORT, BUTTON_1_PIN, GPIO_INPUT);
    gpioPinMode(BUTTON_2_PORT, BUTTON_2_PIN, GPIO_INPUT);
}
uint8_t button1Check(void)
{
    uint8_t buttonStatus = GPIO_HIGH;
	static uint8_t buttonState = GPIO_HIGH;
	static uint8_t buttonLastState = GPIO_HIGH;
	uint32_t lastDeboundTime;
	uint32_t delayDebound = BUTTON_DEBOUND_DELAY_X100MS;
    uint8_t buttonDisable = 0;
	uint8_t reading = gpioReadPin(BUTTON_1_PORT, BUTTON_1_PIN);
    if(buttonDisable == 0)
    {
        if (reading != buttonLastState)
        {
            if(reading != buttonState)
            {
                buttonState = reading;
                if (buttonState == GPIO_LOW)
                {
                    buttonStatus = GPIO_LOW;
                    buttonDisable = 1;
                }
            }
            lastDeboundTime = rtcGetTimer100MS();
        }
    }
    else
    {
        if ((rtcGetTimer100MS() - lastDeboundTime) > delayDebound)
        {
            buttonDisable = 0;
        }
    }
	buttonLastState = reading;
	return buttonStatus;
}

uint8_t button2Check(void)
{
    uint8_t buttonStatus = GPIO_HIGH;
	static uint8_t buttonState = GPIO_HIGH;
	static uint8_t buttonLastState = GPIO_HIGH;
	uint32_t lastDeboundTime;
	uint32_t delayDebound = BUTTON_DEBOUND_DELAY_X100MS;
    uint8_t buttonDisable = 0;
	uint8_t reading = gpioReadPin(BUTTON_2_PORT, BUTTON_2_PIN);
    if(buttonDisable == 0)
    {
        if (reading != buttonLastState)
        {
            if(reading != buttonState)
            {
                buttonState = reading;
                if (buttonState == GPIO_LOW)
                {
                    buttonStatus = GPIO_LOW;
                    buttonDisable = 1;
                }
            }
            lastDeboundTime = rtcGetTimer100MS();
        }
    }
    else
    {
        if ((rtcGetTimer100MS() - lastDeboundTime) > delayDebound)
        {
            buttonDisable = 0;
        }
    }
	buttonLastState = reading;
	return buttonStatus;
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
