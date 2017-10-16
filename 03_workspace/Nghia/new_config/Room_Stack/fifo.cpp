#include "fifo.h"

uint8_t xFIFO_Init (fifo_t* pFIFO) {
  pFIFO->WP = 0;
  pFIFO->RP = 0;
  pFIFO->Count = 0;
  pFIFO->Size = FIFO_LENGTH;
  return 0;
}

uint8_t xFIFO_Check(fifo_t* pFIFO) {
  uint8_t vTemp = FIFO_NORMAL;
  if ((pFIFO->Count == 0) && (pFIFO->WP == pFIFO->RP))
  {
    vTemp = FIFO_EMPTY;
  }
  if ((pFIFO->Count == (pFIFO->Size)) && (pFIFO->WP == pFIFO->RP))
  {
    vTemp = FIFO_FULL;
  }
  return vTemp;
}

uint8_t xFIFO_Send(uint8_t pBuff[FIFO_LENGTH][FIFO_WIDTH], uint8_t pIn[FIFO_WIDTH], fifo_t* pFIFO) {
  if (xFIFO_Check(pFIFO) == FIFO_FULL)
    return 1;
  else {

    for (int i = 0; i < FIFO_WIDTH; i++) {
      pBuff[pFIFO->WP][i] = pIn[i];
    }
    pFIFO->WP++;
    pFIFO->Count++;
    if (pFIFO->WP == pFIFO->Size)
      pFIFO->WP = 0;
    return 0;
  }
  return 1;
}

uint8_t xFIFO_Get (uint8_t pBuff[FIFO_LENGTH][FIFO_WIDTH], uint8_t pOut[FIFO_WIDTH], fifo_t* pFIFO) {
  if (xFIFO_Check(pFIFO) == FIFO_EMPTY)
    return 1;
  else {
    for (int i = 0; i< FIFO_WIDTH; i++) {
      pOut[i] = pBuff[pFIFO->RP][i];
    }
    pFIFO->RP++;
    pFIFO->Count--;
    if (pFIFO->RP == pFIFO->Size)
      pFIFO->RP = 0;
    return 0; 
  }
  return 1;
}

