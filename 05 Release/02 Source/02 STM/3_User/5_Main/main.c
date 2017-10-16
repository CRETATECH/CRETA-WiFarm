/*******************************************************************************
  * @filename   : main.c
  * @author     : HgN
  * @last update: July 19th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "stm8s.h"
#include "StateMachine.h"
/******************************************************************************/
/* MAIN FUNCTION */
/******************************************************************************/
void main (void)
{
    stateInit();
    /* Watchdog init */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(0xFF);
    IWDG_Enable();
    while(1)
    {
        stateUpdate();
        stateMachine();
        IWDG_ReloadCounter();
    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  /*
  while (1)
  {
  }
  */
}
#endif