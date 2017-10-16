/**
  ******************************************************************************
  * HEADER FILE OF STM8 STATE MACHINE
  * File        stateMachine.h
  * Author      HgN
  * Version     V1.0.0
  * Date        May   25th, 2017
  * Last update May   25th, 2017
  ******************************************************************************
  */

#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

/* Include -------------------------------------------------------------------*/
#include "layerHardware.h"

/* Define --------------------------------------------------------------------*/
#define STATE_IDLE 0x00
#define STATE_UART 0x01
#define STATE_SENSOR 0x02

#define FUNCTION_DATA   0x05
#define FUNCTION_DIM    0x01
#define FUNCTION_RTC    0x02
#define FUNCTION_TIME   0x03
#define FUNCTION_STATUS 0x0b

#define STATUS_OK       0x00

/* Public Functions ----------------------------------------------------------*/
void stateUpdate(void);
void stateMachine(void);

#endif
