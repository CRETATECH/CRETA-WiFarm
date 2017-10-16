#ifndef __BH1750_H__
#define __BH1750_H__

#include "i2c.h"

void bh1750Init();
uint16_t bh1750Read();

#endif //BH1750