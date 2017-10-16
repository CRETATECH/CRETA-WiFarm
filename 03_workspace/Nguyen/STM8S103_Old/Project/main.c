#include "stm8s.h"
#include "layerHardware.h"
#include "stateMachine.h"
#include <stdbool.h>

void main( void )
{
    // Clock Init
    
    CLK_DeInit();
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    CLK_AdjustHSICalibrationValue(CLK_HSITRIMVALUE_0);
    
    
    // Module Init, include watch dog init
    deviceInit();
    //watchdogInit();

    while(1)
    {
        stateUpdate();
        //watchdogRefresh();
        stateMachine();
        //watchdogRefresh();
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
