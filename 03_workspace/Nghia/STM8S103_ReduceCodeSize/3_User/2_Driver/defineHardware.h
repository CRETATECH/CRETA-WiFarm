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
//<! Buzzer define
#define BUZZER_PORT     GPIO_PORTD
#define BUZZER_PIN      GPIO_PIN_4

//<! SHT1x define
#define SHT_PORT        GPIO_PORTC
#define SHT_SCK_PIN     GPIO_PIN_5
#define SHT_SDA_PIN     GPIO_PIN_6

//<! DHTxx define
#define DHT_DATA_PORT   GPIO_PORTA
#define DHT_DATA_PIN    GPIO_PIN_3

//<! Device define
#define DEVICE_ON       100
#define DEVICE_OFF      0

#define DEVICE_NONE     0x00
#define DEVICE_1        0x01
#define DEVICE_2        0x02
#define DEVICE_3        0x03

#define IS_DEVICE_OK(device)    ((device == DEVICE_1) || \
                                 (device == DEVICE_2) || \
                                 (device == DEVICE_3))

#define DEVICE_LUX      4
#define DEVICE_TEMP     5
#define DEVICE_HUMID    6

#define DEVICE_1_PORT	GPIO_PORTD
#define DEVICE_1_PIN	GPIO_PIN_2
#define DEVICE_2_PORT	GPIO_PORTD
#define DEVICE_2_PIN	GPIO_PIN_3
#define DEVICE_3_PORT	GPIO_PORTC
#define DEVICE_3_PIN	GPIO_PIN_7

//<! Button define
#define BUTTON_NONE     0
#define BUTTON_1        1
#define BUTTON_2        2

#define BUTTON_1_PORT   GPIO_PORTC
#define BUTTON_1_PIN    GPIO_PIN_3
#define BUTTON_2_PORT   GPIO_PORTC
#define BUTTON_2_PIN    GPIO_PIN_4
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
