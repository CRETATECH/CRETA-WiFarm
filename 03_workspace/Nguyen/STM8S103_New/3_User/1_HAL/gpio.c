/*******************************************************************************
  * @filename   : gpio.c
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* REVISION HISTORY */
// March 17: Initialization.
// July 19: Update Project, clean file
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
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
/* PUBLIC VARIABLES */
/******************************************************************************/

/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief To init gpio pin
 * @param pPort
 * @param pPin
 * @param pMode
 */
void gpioPinMode(uint16_t pPort, uint8_t pPin, uint8_t pMode)
{
    GPIO_TypeDef *Port;
    Port = (GPIO_TypeDef*)pPort;
    GPIO_Init(Port, pPin, pMode);
}

/**
 * @brief To write 1 byte to a port
 * @param pPort
 * @param pPortVal
 */
void gpioWritePort(uint16_t pPort, uint8_t pPortVal)
{
    GPIO_TypeDef *Port;
    Port = (GPIO_TypeDef*)pPort;
    GPIO_Write(Port, pPortVal);
}

/**
 * @brief To write a pin high/low
 * @param pPort
 * @param pPin
 * @param pPinVal
 */
void gpioWritePin(uint16_t pPort, uint8_t pPin, uint8_t pPinVal)
{
    GPIO_TypeDef *Port;
    Port = (GPIO_TypeDef*)pPort;
    if (pPinVal == GPIO_HIGH)
        GPIO_WriteHigh(Port, pPin);
    else GPIO_WriteLow(Port, pPin);
}


/**
 * @brief To read value of a gpio input pin
 * @param pPort
 * @param pPin
 * @ret
 *      GPIO_HIGH
 *      GPIO_LOW
 */
uint8_t gpioReadPin(uint16_t pPort, uint8_t pPin)
{
    GPIO_TypeDef *Port;
    Port = (GPIO_TypeDef*)pPort;
    if (GPIO_ReadInputPin(Port, pPin) == 0)
        return GPIO_LOW;
    else return GPIO_HIGH;
}


/**
 * @brief To setup gpio interrupt
 * @param pPort
 * @param pPin
 * @param pIntType
 */
void gpioIntConfig(uint16_t pPort, uint8_t pPin, uint8_t pIntType)
{
    GPIO_TypeDef *Port;
    Port = (GPIO_TypeDef*)pPort;
    EXTI_Port_TypeDef IntPort;
    switch (pPort)
    {
    case GPIO_PORTA:
        IntPort = EXTI_PORT_GPIOA;
        break;
    case GPIO_PORTB:
        IntPort = EXTI_PORT_GPIOA;
        break;
    case GPIO_PORTC:
        IntPort = EXTI_PORT_GPIOC;
        break;
    case GPIO_PORTD:
        IntPort = EXTI_PORT_GPIOD;
        break;
    }
    GPIO_Init(Port, pPin, GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity(IntPort, pIntType);
}

/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
