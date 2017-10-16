
#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "stm8s.h"

#define LED_GPIO_PORT  GPIOD        // khai bao lai port muon test (GPIOA)
#define LED_GPIO_PIN   GPIO_PIN_2    // khai bao lai pin muon test  (GPIO_PIN_1)

void Delay (uint16_t nCount);
void blinky();
void gpioInit();

#endif 