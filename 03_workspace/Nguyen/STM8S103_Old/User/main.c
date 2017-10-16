#include "stm8s.h"
#include "stm8s_i2c.h"

unsigned int countloop = 0;
uint32_t freq;
uint8_t data_high;
uint8_t data_low;
uint16_t data;

void delay(uint16_t nCount);

void main( void )
{
    CLK_DeInit();
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    CLK_AdjustHSICalibrationValue(CLK_HSITRIMVALUE_0);
    freq = CLK_GetClockFreq();   
    
    u8 Input_Clock = 0x00;
    Input_Clock = CLK_GetClockFreq()/1000000;
    I2C_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
    I2C_Cmd( ENABLE);
    /* sEE_I2C configuration after enabling it */
    I2C_Init(100000, 0x23, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT,Input_Clock);
    
    //start i2c
    while(I2C_GetFlagStatus( I2C_FLAG_BUSBUSY)==SET);
    I2C_GenerateSTART(ENABLE);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(0x46,I2C_DIRECTION_TX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData( 0x01);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(ENABLE);
    I2C->SR1;		I2C->SR3;
    
    while(I2C_GetFlagStatus( I2C_FLAG_BUSBUSY)==SET);
    I2C_GenerateSTART(ENABLE);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(0x46,I2C_DIRECTION_TX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData( 0x07);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(ENABLE);
    I2C->SR1;		I2C->SR3;
  

    
    while(1)
    {
    while(I2C_GetFlagStatus( I2C_FLAG_BUSBUSY)==SET);
    I2C_GenerateSTART(ENABLE);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(0x46,I2C_DIRECTION_TX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData( 0x23);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(ENABLE);
    I2C->SR1;		I2C->SR3;
    
    delay(500000);
    
    I2C_GenerateSTART( ENABLE);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(0x46,I2C_DIRECTION_RX);
    while(I2C_GetFlagStatus( I2C_FLAG_ADDRESSSENTMATCHED) == RESET);
    //I2C_AcknowledgeConfig(I2C_ACK_NONE); //TODO: check
    I2C_AcknowledgeConfig( I2C_ACK_CURR);
    I2C->SR1;		I2C->SR3;
    while (I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET);
    data_high=I2C_ReceiveData();
    while (I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET);
    data_low=I2C_ReceiveData();
    I2C_GenerateSTOP(ENABLE);
    while(I2C->CR2 & I2C_CR2_STOP);
    I2C_AcknowledgeConfig( I2C_ACK_CURR);
    data = ((uint16_t)data_high<<8) | (uint16_t)data_low;
    }
    
}


void delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
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