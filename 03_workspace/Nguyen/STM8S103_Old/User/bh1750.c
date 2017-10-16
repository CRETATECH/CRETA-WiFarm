#include "bh1750.h"

/* Private Function */
void bh1750Delay(uint16_t nCount);

void bh1750Init(void)
{
    i2cBeginTransmission(0x46);
    i2cWrite(0x01);
    i2cEndTransmission();
    i2cBeginTransmission(0x46);
    i2cWrite(0x07);
    i2cEndTransmission();
}

uint16_t bh1750Read(void)
{
    uint8_t data_high, data_low;
    uint16_t data;
    i2cBeginTransmission(0x46);
    i2cWrite(0x23);
    i2cEndTransmission();
    bh1750Delay(0xFFFF);
    i2cRequestFrom(0x46, 2);
    data_high = i2cRead();
    data_low  = i2cRead();
    data = (uint16_t)(data_high<<8) | (uint16_t)data_low;
    return data;
}

void bh1750Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
