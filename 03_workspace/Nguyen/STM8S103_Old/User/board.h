#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm8s.h"
#include "gpio.h"
#include "timer.h"
#include "i2c.h"
#include "uart.h"
#include "bh1750.h"
#include "sht1x.h"
#include "pwm.h"

#define CMD_MAX_LENGTH  20
#define FRAME_LENGTH    11

uint8_t cmdGet();
void cmdDoRequest();
void cmdSensorReturn(uint8_t deviceAddress, uint16_t deviceData);
uint32_t timerGetSecond();

#endif