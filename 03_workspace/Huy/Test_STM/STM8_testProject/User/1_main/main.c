#include "gpio.h"
#include "DS1037.h"
#include "I2C.h"

int status,status1;

int main()
{
    /************** System Clock ************************/
    
    CLK_DeInit();
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV4);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV4);
    CLK_AdjustHSICalibrationValue(CLK_HSITRIMVALUE_0);
    /************** Init ************************/
    
    gpioInit();
    init_i2c();
    while(1) 
    {
 
    /************** Start Test ************************/
    //////////////// buzzer ////////////////
    buzzer();
    //////////////// Led ////////////////
    ledOff();
    ledOn();
    Delay();
    ledOff();
    Delay();
    //////////////// relay ////////////////
    relay_1();
    
    relay_2();
    //////////////// button ////////////////
    
    sw_1Check();
    sw_2Check();
    //Delay();
    //////////////// I2C ////////////////
    
    // send lan 1
    /*if(writeDS1307(0x01))
    { // sucess
      ledOn();
      Delay();
      ledOff();
      Delay();
    }
    // read lan 1
    if(readDS1307(0x01))
    { // sucess
      ledOn();
      Delay();
      ledOff();
      Delay();
    }
    // send lan 2
    if(writeDS1307(0x02))
    { // sucess
      ledOn();
      Delay();
      ledOff();
      Delay();
    }
    // read lan 2
    if(readDS1307(0x02))
    { // sucess
      ledOn();
      Delay();
      ledOff();
      Delay();
    }
    */
      //blinky();
    }
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */
