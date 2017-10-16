/**
  ******************************************************************************
  * HEADER FILE OF STM8 BOARD HARDWARE LAYER
  * File        layerHardware.h
  * Author      HgN
  * Version     V1.0.0
  * Date        May   25th, 2017
  * Last update May   25th, 2017
  ******************************************************************************
  */

#ifndef __LAYER_HARDWARE__
#define __LAYER_HARDWARE__

/* Include -------------------------------------------------------------------*/
#include "uart.h"
#include "gpio.h"
#include "timer.h"
#include "bh1750.h"
#include "sht1x.h"
#include "ds1307.h"

/* Define --------------------------------------------------------------------*/
#define DEVICE_NONE		0xFF
#define DEVICE_ONOFF_1  0x01
#define DEVICE_ONOFF_2  0x02
#define DEVICE_ONOFF_3  0x03
#define DEVICE_DIM_1    0x04
#define DEVICE_TEMP     0x05
#define DEVICE_HUMID    0x06
#define DEVICE_LUX      0x07

#define DEVICE_1_PORT	GPIO_PORTD
#define DEVICE_1_PIN	PIN_2
#define DEVICE_2_PORT	GPIO_PORTD
#define DEVICE_2_PIN	PIN_3
#define DEVICE_3_PORT	GPIO_PORTA
#define DEVICE_3_PIN	PIN_7

#define FLASH_MIN_ADDR  (uint16_t)0x004000
#define FLASH_MAX_ADDR  (uint16_t)0x0047FF

/* Public Functions ----------------------------------------------------------*/
void deviceInit(void);
uint8_t deviceGetStatus(uint8_t pDevice);
uint32_t timerGetSecond(void);
void watchdogInit(void);
void watchdogRefresh(void);

#endif
