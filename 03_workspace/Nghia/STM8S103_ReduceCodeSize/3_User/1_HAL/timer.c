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
void timer2Init(uint16_t pPeriod)
{
    CLK->PCKENR1 |= (uint8_t)((uint8_t)1 << ((uint8_t)0x05 & (uint8_t)0x0F));
    TIM2->PSCR = (uint8_t)(TIMER_PRESCALER);
    TIM2->ARRH = (uint8_t)(pPeriod >> 8);
    TIM2->ARRL = (uint8_t)(pPeriod);
    TIM2->CR1 |= (uint8_t)TIM2_CR1_CEN;
}
/**
 * @brief To enable timer interrupt
 * @param None
 */
void timer2IntEnable(void)
{
    TIM2->IER |= (uint8_t)0x01;
}

/**
 * @brief To disable timer interrupt
 * @param None
 */
void timer2IntDisable(void)
{
    TIM2->IER &= (uint8_t)(~0x01);
}

/**
 * @brief   To clear timer interrupt flag
 * @param   None
 */
void timer2IntFlagClear(void)
{
    TIM2->SR1 = (uint8_t)(~TIM2_IT_UPDATE);
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
