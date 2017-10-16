/**
  ******************************************************************************
  * SOURCE FILE BASE MACHINE GPIO
  * File        gpio.c 
  * Author      HgN
  * Version     V1.0.0
  * Last update March 18th, 2017     
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "gpio.h"

/* Functions -----------------------------------------------------------------*/

/* GPIO PIN MODE
 * Set mode for gpio pin
 * ==ARGS==
 * __pPort: GPIO_PORTx (A->D)
 * __pPin: PIN_x (0->7, ALL)
 * __pMode: GPIO_OUTPUT/GPIO_INPUT
 */
void gpioPinMode(uint16_t pPort, uint8_t pPin, uint8_t pMode)
{
    GPIO_TypeDef *Port;
    Port = (GPIO_TypeDef*)pPort;
    GPIO_Init(Port, pPin, pMode);
}


/* GPIO WRITE PORT
 * Write 1 byte to 1 port
 * ==ARGS==
 * __pPort: GPIO_PORTx (A->D)
 * __pPortVal: 8-bit value
 */
void gpioWritePort(uint16_t pPort, uint8_t pPortVal)
{
    GPIO_TypeDef *Port;
    Port = (GPIO_TypeDef*)pPort;
    GPIO_Write(Port, pPortVal);
}


/* GPIO WRITE PIN
 * Write to a specific pin
 * ==ARGS==
 * __pPort: GPIO_PORTx (A->D)
 * __pPin: PIN_x (0->7, ALL)
 * __pPinVal: HIGH/LOW
 */
void gpioWritePin(uint16_t pPort, uint8_t pPin, uint8_t pPinVal)
{
    GPIO_TypeDef *Port;
    Port = (GPIO_TypeDef*)pPort;
    if (pPinVal == HIGH)
        GPIO_WriteHigh(Port, pPin);
    else GPIO_WriteLow(Port, pPin);
}


/* GPIO READ PIN
 * Read value of specific pin
 * ==ARGS==
 * __pPort: GPIO_PORTx (A->D)
 * __pPin: PIN_x (0->7, ALL)
 * ==RETURN==
 * __Pin value: HIGH/LOW
 */
uint8_t gpioReadPin(uint16_t pPort, uint8_t pPin)
{
    GPIO_TypeDef *Port;
    Port = (GPIO_TypeDef*)pPort;
    if (GPIO_ReadInputPin(Port, pPin) == 0)
        return LOW;
    else return HIGH;
}


/* GPIO INTERRUPT MODE
 * Enable a specific gpio interrupt
 * ==ARGS==
 * __pPort: GPIO_PORTx (A->D)
 * __pPin: PIN_x (0->7, ALL)
 * __pIntMode: EXINT_RISE_EDGE/EXINT_FALL_EDGE/EXINT_BOTH_EDGE
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