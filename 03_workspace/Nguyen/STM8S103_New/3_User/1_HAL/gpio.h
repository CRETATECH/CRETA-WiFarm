/*******************************************************************************
  * @filename   : gpio.h
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

#ifndef GPIO_H
#define GPIO_H

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "stm8s.h"

/******************************************************************************/
/* PUBLIC TYPEDEFS */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/
#define GPIO_HIGH    1
#define GPIO_LOW     0

#define GPIO_PORTA (uint16_t)0x5000
#define GPIO_PORTB (uint16_t)0x5005
#define GPIO_PORTC (uint16_t)0x500A
#define GPIO_PORTD (uint16_t)0x500F

#define GPIO_PIN_0   (uint8_t)0x01
#define GPIO_PIN_1   (uint8_t)0x02
#define GPIO_PIN_2   (uint8_t)0x04
#define GPIO_PIN_3   (uint8_t)0x08
#define GPIO_PIN_4   (uint8_t)0x10
#define GPIO_PIN_5   (uint8_t)0x20
#define GPIO_PIN_6   (uint8_t)0x40
#define GPIO_PIN_7   (uint8_t)0x80
#define GPIO_PIN_ALL (uint8_t)0xFF

#define GPIO_OUTPUT  (uint8_t)0xE0
#define GPIO_INPUT   (uint8_t)0x00

#define EXINT_RISE_EDGE (uint8_t)0x01
#define EXINT_FALL_EDGE (uint8_t)0x02
#define EXINT_BOTH_EDGE (uint8_t)0x03

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES */
/******************************************************************************/
void gpioPinMode(uint16_t pPort, uint8_t pPin, uint8_t pMode);
void gpioIntConfig(uint16_t pPort, uint8_t pPin, uint8_t IntType);
void gpioWritePort(uint16_t pPort, uint8_t pPortVal);
void gpioWritePin(uint16_t pPort, uint8_t pPin, uint8_t pPinVal);
uint8_t gpioReadPin(uint16_t pPort, uint8_t pPin);

#endif
