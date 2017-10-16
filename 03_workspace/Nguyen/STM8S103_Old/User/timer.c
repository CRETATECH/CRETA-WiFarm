/**
  ******************************************************************************
  * SOURCE FILE BASE MACHINE TIMER2
  * File        timer.c 
  * Author      HgN
  * Version     V1.0.0
  * Date        March 21th, 2017
  * Last update March 21th, 2017
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "timer.h"

/* Functions -----------------------------------------------------------------*/

void timerInit(uint16_t pPeriod)
{
    TIM2_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
    TIM2_TimeBaseInit(TIMER_PRESCALER, pPeriod);
    TIM2_Cmd(ENABLE);
}

void timerIntEnable()
{
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
}