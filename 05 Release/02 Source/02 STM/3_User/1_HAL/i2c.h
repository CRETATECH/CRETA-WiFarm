/*******************************************************************************
  * @filename   : i2c.h
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

#ifndef		I2C_H
#define		I2C_H

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
#define I2C_BUFFER_MAX      256
#define I2C_CLOCK_STD       100000
#define I2C_CLOCK_FAST      400000

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES */
/******************************************************************************/
void i2cInit(uint32_t pSpeed, uint8_t pOwnAddr);
int i2cBeginTransmission(uint8_t pAddr);
void i2cEndTransmission(void);
int i2cRequestFrom(uint8_t pAddr, uint8_t *pBuff, uint8_t pLength);
int i2cWrite(uint8_t pData);
uint8_t i2cRead();

#endif
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
