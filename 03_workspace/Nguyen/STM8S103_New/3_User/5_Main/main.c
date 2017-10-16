#include "stm8s.h"
#include "StateMachine.h"

//#define DEBUG

void main (void)
{
    // Clock Init
    CLK_DeInit();
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    CLK_AdjustHSICalibrationValue(CLK_HSITRIMVALUE_0);

    //<! UART initialize
    uartInit(115200);
    //<! Internal eeprom initialize
    eepromInit();
    //<! I2C initialize
    i2cInit(I2C_CLOCK_STD, 0x23);
    //<! Device initialize
    deviceInit();
    //<! SHT initialize
    shtInit();
    //<! RTC func initialize
    rtcInit();
    //<! Global interrupt enable
    enableInterrupts();

    while(1)
    {
        stateUpdate();
        stateMachine();
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