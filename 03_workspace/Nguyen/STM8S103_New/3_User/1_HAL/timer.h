/*******************************************************************************
  * @filename   : timer.h
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/
#ifndef TIMER_H
#define TIMER_H

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

#define TIMER_PRESCALER     Prescale_128
/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES */
/******************************************************************************/
void timerInit(uint16_t pPeriod);
void timerIntEnable(void);
void timerIntDisable(void);
void timerIntFlagClear(void);

#endif
