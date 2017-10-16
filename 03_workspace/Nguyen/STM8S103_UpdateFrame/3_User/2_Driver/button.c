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
uint8_t buttonCheck(uint8_t pButton)
{
    static uint8_t buttonLastStatus = GPIO_HIGH;
    uint8_t buttonStatus;
    //<! Get button port and pin
    uint16_t vPort;
    uint8_t vPin;
    switch(pButton)
    {
    case BUTTON_1:
        vPort = BUTTON_1_PORT;
        vPin = BUTTON_1_PIN;
        break;
    case BUTTON_2:
        vPort = BUTTON_2_PORT;
        vPin = BUTTON_2_PIN;
        break;
    default:
        break;
    }
    buttonStatus = gpioReadPin(vPort, vPin);
    if(buttonLastStatus != buttonStatus)
    {
        if(buttonStatus == GPIO_LOW)
        {
            buttonLastStatus = buttonStatus;
            return GPIO_LOW;
        }
    }
    buttonLastStatus = buttonStatus;
    return GPIO_HIGH;
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
