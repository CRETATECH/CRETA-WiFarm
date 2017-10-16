/*******************************************************************************
  * @filename   : dhtxx.h
  * @author     : HgN
  * @last update: August 17th, 2017
  */
/******************************************************************************/

#ifndef DHT_H
#define DHT_H

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "defineHardware.h"
/******************************************************************************/
/* PUBLIC TYPEDEFS */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES */
/******************************************************************************/
void dhtInit(void);
int dhtStart(void);
int dhtRead(float* pHumid, float* pTemp);

#endif
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/