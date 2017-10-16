/**
  ******************************************************************************
  * HEADER FILE OF HARDWARE DEFINE
  * File        defineHardware.h
  * Author      HgN
  * Version     V1.0.0
  * Date        March 18th, 2017
  * Last update May   25th, 2017
  ******************************************************************************
  */


#ifndef	DEFINE_HARDWARE_H
#define DEFINE_HARDWARE_H

#include "stm8s.h"
#include <stdbool.h>

/* Define SYS-UART */
#define	B9600		     9600
#define	B57600		     57600
#define	B115200		     115200

/* Define SYS-I2C */
#define Hz50000          50000
#define Hz100000         100000
#define Hz200000         200000

/* Define SYS-GPIO */
  

/* Define SYS-TIMER */
#define Prescale_1      ((uint8_t)0x00)
#define Prescale_2      ((uint8_t)0x01)
#define Prescale_4      ((uint8_t)0x02)
#define Prescale_8      ((uint8_t)0x03)
#define Prescale_16     ((uint8_t)0x04)
#define Prescale_32     ((uint8_t)0x05)
#define Prescale_64     ((uint8_t)0x06)
#define Prescale_128    ((uint8_t)0x07)
#define Prescale_256    ((uint8_t)0x08)
#define Prescale_512    ((uint8_t)0x09)
#define Prescale_1024   ((uint8_t)0x0A)
#define Prescale_2048   ((uint8_t)0x0B)
#define Prescale_4096   ((uint8_t)0x0C)
#define Prescale_8192   ((uint8_t)0x0D)
#define Prescale_16384  ((uint8_t)0x0E)
#define Prescale_32768  ((uint8_t)0x0F)


/* Define SYSTEMS */
#define	BAUD_SPEED			B115200

#define	I2C_BUFFER_MAX	    256
#define I2C_CLOCK_FREQ      100000
#define I2C_OWN_ADDRESS     0x23

#define TIMER_PRESCALER     Prescale_128

#define	UART_BUFFER_MAX		256

#define SENSOR_GAP          30

#define	SYS_OK		1
#define	SYS_ERR		0



#endif
