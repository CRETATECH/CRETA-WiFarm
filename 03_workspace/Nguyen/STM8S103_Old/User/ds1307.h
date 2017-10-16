/**
  ******************************************************************************
  * HEADER FILE FOR MODLULE REALTIME DS1307
  * File        ds1307.h
  * Author      HgN
  * Version     V1.0.0
  * Date        March 18th, 2017
  * Last update March 18th, 2017
  ******************************************************************************
  */


#ifndef __DS1307_H__
#define __DS1307_H__

/* Include -------------------------------------------------------------------*/
#include "i2c.h"

/* Define --------------------------------------------------------------------*/
#define DS1307_ADDRESS   (uint8_t)0xD0
#define DS1307_REG_SEC   0x00
#define DS1307_REG_MIN   0x01
#define DS1307_REG_HOUR  0x02
#define DS1307_REG_DAY   0x04
#define DS1307_REG_MONTH 0x05
#define DS1307_REG_YEAR  0x06

/* Public Functions ----------------------------------------------------------*/
void ds1307Init(uint8_t pYear, uint8_t pMonth, uint8_t pDay, uint8_t pHour, uint8_t pMin);
uint8_t ds1307Read(uint8_t pRegAdd);

#endif //BH1750