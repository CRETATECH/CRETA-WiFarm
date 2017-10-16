/*******************************************************************************
  * @filename   : defineHardware.h
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "stm8s.h"
#include "gpio.h"
#include "i2c.h"
#include "uart.h"
#include "timer.h"

/******************************************************************************/
/* PUBLIC TYPEDEFS */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/
//<! SHT1x define
#define SHT_PORT        GPIOC
#define SHT_SCK_PIN     GPIO_PIN_4
#define SHT_SDA_PIN     GPIO_PIN_5

//<! Device define
#define DEVICE_ON   100
#define DEVICE_OFF  0

#define DEVICE_NONE     0
#define DEVICE_1        1
#define DEVICE_2        2
#define DEVICE_3        3
#define DEVICE_LUX      4
#define DEVICE_TEMP     5
#define DEVICE_HUMID    6

#define DEVICE_1_PORT	GPIO_PORTD
#define DEVICE_1_PIN	GPIO_PIN_2
#define DEVICE_2_PORT	GPIO_PORTD
#define DEVICE_2_PIN	GPIO_PIN_3
#define DEVICE_3_PORT	GPIO_PORTA
#define DEVICE_3_PIN	GPIO_PIN_7

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
