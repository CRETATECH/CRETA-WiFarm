#ifndef __I2C_H__
#define __I2C_H__

#include "stm8s.h"
#include "stdio.h"
#include <stdbool.h>
#include "DS1037.h" 
#include "stm8s.h"

#define EXIT_FAILURE 0
#define EXIT_SUCCESS 1

void init_i2c(void);
int i2c_WriteByte(uint8_t SLAVE_ADDRESS, uint8_t ReadAdress, uint8_t data);
int i2c_ReadByte(uint8_t SLAVE_ADDRESS, uint8_t ReadAdress, uint8_t *pBuffer);
void i2cEndTransmission(void);





#endif 