/**
  ******************************************************************************
  * HEADER FILE OF PWM MODULE
  * File        pwm.c
  * Author      HgN
  * Version     V1.0.0
  * Date        March 18th, 2017
  * Last update May   25th, 2017
  ******************************************************************************
  */

#ifndef __PWM_H__
#define __PWM_H__

/* Include -------------------------------------------------------------------*/
#include "defineHardware.h"

/* Functions -----------------------------------------------------------------*/
void pwmInit(uint8_t pDutyCycle);

#endif