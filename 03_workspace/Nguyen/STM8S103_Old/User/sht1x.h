#ifndef SHT_H
#define SHT_H

/* Include */
#include "stm8s.h"
#include "defineHardware.h"

/* Define */
#define SHT_PORT        GPIOC
#define SHT_SCK_PIN     GPIO_PIN_4
#define SHT_SDA_PIN     GPIO_PIN_5
#define SHT_CMD_TEMP    (uint8_t)0x03
#define SHT_CMD_HUMID   (uint8_t)0x05

/* Public Function */
void shtInit();
uint8_t shtReadTemp(float *temp);
uint8_t shtReadHumid(float *humid);

#endif
