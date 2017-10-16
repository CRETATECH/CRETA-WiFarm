/**
  ******************************************************************************
  * HEADER FILE BASE MACHINE UART
  * File        uart.c 
  * Author      HgN
  * Version     V1.0.0
  * Date        March 18th, 2017
  * Last update March 18th, 2017
  ******************************************************************************
  */

#ifndef		UART_H
#define		UART_H

/* Include -------------------------------------------------------------------*/
#include "defineHardware.h"

/* Public Functions ----------------------------------------------------------*/
uint8_t	uartInit(void);
uint8_t	uartRead(void);
uint8_t uartAvailable(void);
void uartWrite(uint8_t * pData, uint8_t pLength);
void uartWriteByte(uint8_t pByte);

#endif		//UART_H
