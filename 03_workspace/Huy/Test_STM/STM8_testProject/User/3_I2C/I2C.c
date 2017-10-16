
#include "I2C.h"



uint32_t vTimeOut = 0xFFFF;
/**
  * Function Name: init_i2c
  * Description:   init_i2c
  * Input:         none
  * Output:        none
  * Return:        void
  */
void init_i2c(void)
{
     /* DeInit */
    I2C_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
    uint8_t Input_Clock = 0x00;
    Input_Clock = CLK_GetClockFreq()/1000000;
    I2C_Cmd(ENABLE);
    I2C_Init(100000, 0x00, I2C_DUTYCYCLE_2,
             I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);
}
/**
  * Function Name: i2c_WriteByte
  * Description:   write one byte via I2C 
  * Input:         SLAVE_ADDRESS //  ReadAdress
  * Output:       
  * Return:        1 if sucess 0 if fail
  */
int i2c_WriteByte(uint8_t SLAVE_ADDRESS, uint8_t ReadAdress, uint8_t data)
{
    
    while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    
    /* Send START condition */
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    
     /* Send slave Address for write */
    I2C_Send7bitAddress(SLAVE_ADDRESS, I2C_DIRECTION_TX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    /* Send data for write */
    I2C_SendData(ReadAdress);
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    
    I2C_SendData(data);
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
        
}
/**
  * Function Name: i2c_ReadByte
  * Description:   Read one byte via I2C 
  * Input:         SLAVE_ADDRESS //  ReadAdress
  * Output:       
  * Return:        1 if sucess 0 if fail
  */
int i2c_ReadByte(uint8_t SLAVE_ADDRESS, uint8_t ReadAdress, uint8_t *pBuffer)
{
    
      /* Send START condition */
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
     /* Send slave Address for write */
    I2C_Send7bitAddress(SLAVE_ADDRESS, I2C_DIRECTION_TX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    /* Send data for write */
    I2C_SendData(ReadAdress);
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
  
      /* restart */
    I2C_GenerateSTART(ENABLE);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    //send ds1307 address for read
    I2C_Send7bitAddress(SLAVE_ADDRESS,I2C_DIRECTION_RX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
   
    while (I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET)
        {
            vTimeOut--;
            if(0 == vTimeOut)
                return EXIT_FAILURE;
        }
        *pBuffer =  I2C_ReceiveData();
        return EXIT_SUCCESS;
    
}
/**
  * Function Name: i2cEndTransmission(void)
  * Description:   i2cEndTransmission
  * Input:         none
  * Output:        none
  * Return:        void
  */
void i2cEndTransmission(void)
{
    I2C_GenerateSTOP(ENABLE);
    I2C->SR1;		I2C->SR3;
}
