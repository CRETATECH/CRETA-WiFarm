/*******************************************************************************
  * @filename   : timer.c
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* REVISION HISTORY */
// March 17: Initialization.
// July 19: Update Project, clean file
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "timer.h"
#include <stdlib.h>
#include <stdbool.h>

/******************************************************************************/
/* LOCAL TYPEDEFS */
/******************************************************************************/

/******************************************************************************/
/* LOCAL DEFINES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC VARIABLES */
/******************************************************************************/

/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief To init timer2
 * @param pPeriod
 */
void timerInit(uint16_t pPeriod)
{
    TIM2_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
    TIM2_TimeBaseInit(TIMER_PRESCALER, pPeriod);
    TIM2_Cmd(ENABLE);
}
/**
 * @brief To enable timer interrupt
 * @param None
 */
void timerIntEnable(void)
{
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
}

/**
 * @brief To disable timer interrupt
 * @param None
 */
void timerIntDisable(void)
{
    TIM2_ITConfig(TIM2_IT_UPDATE, DISABLE);
}

/**
 * @brief To clear timer interrupt flag
 * @param None
 */
void timerIntFlagClear(void)
{
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
