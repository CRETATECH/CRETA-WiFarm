/**
  ******************************************************************************
  * SOURCE FILE BASE MACHINE PWM
  * File        pwm.c
  * Author      HgN
  * Version     V1.0.0
  * Date        March 18th, 2017
  * Last update May   25th, 2017
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "pwm.h"

/* Functions -----------------------------------------------------------------*/

/* PWM INIT
 * Run timer 1 pwm module with sys freq.
 * ==ARGS==
 * __pDutyCycle: pwm duty cycle (0->100)
 */
void pwmInit(uint8_t pDutyCycle)
{
    if (pDutyCycle > 100)
        pDutyCycle = 100;
    float Pulse = (4096*(float)pDutyCycle)/100;
    TIM1_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
    TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 4095, 0);
    TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
               (uint16_t)Pulse, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_RESET);
    TIM1_Cmd(ENABLE);
    TIM1_CtrlPWMOutputs(ENABLE);
}