/**
  ******************************************************************************
  * HEADER FILE BASE MACHINE I2C
  * File        i2c.h 
  * Author      HgN
  * Version     V1.0.0
  * Date        March 18th, 2017
  * Last update May   25th, 2017
  ******************************************************************************
  */

#ifndef		I2C_H
#define		I2C_H

/* Include -------------------------------------------------------------------*/
#include "defineHardware.h"

/* Functions -----------------------------------------------------------------*/
void i2cInit();
uint8_t	i2cAvailable(void);
void i2cBeginTransmission(uint8_t pAddr);
void i2cEndTransmission(void);
void i2cRequestFrom(uint8_t pAddr, uint8_t pLength);
void i2cWrite(uint8_t pData);
uint8_t i2cRead();


#endif		
