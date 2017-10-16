#ifndef __FIFO_H
#define __FIFO_H

#include <stdint.h>
#include <stdio.h>

#define FIFO_FULL   0
#define FIFO_EMPTY  1
#define FIFO_NORMAL 2

#define FIFO_LENGTH 5
#define FIFO_WIDTH 11

typedef struct {
  int WP;
  int RP;
  uint8_t Count;
  uint8_t Size;
//  uint8_t full_flag;
//  uint8_t empty_flag;
}fifo_t;

uint8_t xFIFO_Init (fifo_t* pFIFO);
uint8_t xFIFO_Check(fifo_t* pFIFO);
uint8_t xFIFO_Send(uint8_t pBuff[FIFO_LENGTH][FIFO_WIDTH], uint8_t *pIn[FIFO_WIDTH], fifo_t* pFIFO);
uint8_t xFIFO_Get (uint8_t pBuff[FIFO_LENGTH][FIFO_WIDTH], uint8_t *pOut[FIFO_WIDTH], fifo_t* pFIFO);

#endif
