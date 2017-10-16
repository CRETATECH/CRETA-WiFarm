
#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm8s.h"

#define LED_GPIO_PORT             GPIOC        // khai bao lai port muon test (GPIOA)
#define LED_GPIO_PIN              GPIO_PIN_7    // khai bao lai pin muon test  (GPIO_PIN_1)
#define RELAY_GPIO_PORT           GPIOD
#define RELAY_GPIO_PIN_1          GPIO_PIN_3 
#define RELAY_GPIO_PIN_2          GPIO_PIN_2  
#define BUZZ_GPIO_PORT            GPIOD
#define BUZZ_GPIO_PIN             GPIO_PIN_4
#define SW_GPIO_PORT              GPIOC
#define SW_GPIO_PIN_1             GPIO_PIN_4  
#define SW_GPIO_PIN_2             GPIO_PIN_3  

void Delay();
void blinky();
void gpioInit();
void ledOff();
void ledOn();
void relay_1();
void relay_2();
void buzzer();
int button_1Check();
int button_2Check();
int sw_1Check();
int sw_2Check();

#endif 