/*******************************************************************************
  * @filename   : i2c.c
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
#include "i2c.h"
#include <stdbool.h>
#include <stdlib.h>

/******************************************************************************/
/* LOCAL TYPEDEFS */
/******************************************************************************/

/******************************************************************************/
/* LOCAL DEFINES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC VARIABLES */
/******************************************************************************/
uint8_t gBufferI2C[I2C_BUFFER_MAX];
uint8_t pBufferI2CWrite = 1;
uint8_t pBufferI2CRead = 0;
uint8_t gReceiveByteI2C;

/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief I2C Initialization
 * @param pSpeed
 * @param pSpeed
 * @details
 *          7-bit address, ACK current byte
 *          I2C Buffer/Error Interrupt ENABLE
 *          If fast mode ON (ClkFregHz>100kHz), Duty cycle = T_low/T_high = 2
 */
void i2cInit(uint32_t pSpeed, uint8_t pOwnAddr)
{
    I2C_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
    uint8_t Input_Clock = 0x00;
    Input_Clock = CLK_GetClockFreq()/1000000;
    I2C_Init(pSpeed, pOwnAddr, I2C_DUTYCYCLE_2,
             I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);
    I2C_ITConfig(I2C_IT_BUF, ENABLE);
    I2C_Cmd(ENABLE);
}

/**
 * @brief Check I2C Buffer
 * @ret   Buffer length
 */
uint8_t	i2cAvailable(void){
    uint8_t i2cBufferLength;
    if(pBufferI2CWrite > pBufferI2CRead)
    {
        i2cBufferLength = pBufferI2CWrite - pBufferI2CRead - 1;
    }
    else
    {
        i2cBufferLength = I2C_BUFFER_MAX + pBufferI2CWrite - pBufferI2CRead - 1;
    }
    return i2cBufferLength;
}


/**
 * @brief To begin a transmisstion
 * @param pAddr
 */
void i2cBeginTransmission(uint8_t pAddr)
{
    while(I2C_GetFlagStatus( I2C_FLAG_BUSBUSY)==SET);
    I2C_GenerateSTART(ENABLE);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(pAddr,I2C_DIRECTION_TX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}


/**
 * @brief To send STOP signal to i2c line
 * @param None
 */
void i2cEndTransmission(void)
{
    I2C_GenerateSTOP(ENABLE);
    I2C->SR1;		I2C->SR3;
}

/**
 * @brief To request number of bytes from slave and write to buffer
 * @param pAddr
 * @param pLength
 */
void i2cRequestFrom(uint8_t pAddr, uint8_t pLength)
{
    I2C_GenerateSTART(ENABLE);
    while (I2C_GetLastEvent() != I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(pAddr, I2C_DIRECTION_RX);
    while (I2C_GetLastEvent() != I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
    I2C->SR1;		I2C->SR3;
    while (pLength != 0)
    {
        while (I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET);
        gBufferI2C[pBufferI2CWrite] =  I2C_ReceiveData();
        pBufferI2CWrite++;
        if (I2C_BUFFER_MAX == pBufferI2CWrite)
        {
            pBufferI2CWrite = 0;
        }
        pLength--;
    }
    I2C_GenerateSTOP(ENABLE);
    I2C->SR1;		I2C->SR3;
} // TODO: no buffer overflow solution yet


/**
 * @brief To send 1 byte data to i2c line
 * @param pData
 */
void i2cWrite(uint8_t pData)
{
    I2C_SendData(pData);
    while(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET);
}


/**
 * @brief To read 1 byte from i2c line
 * @ret   The read byte
 */
uint8_t i2cRead()
{
    uint8_t i2cData;
    uint8_t i2cBufferReadTemp = pBufferI2CRead + 1;
    if (i2cBufferReadTemp == I2C_BUFFER_MAX)
    {
        i2cBufferReadTemp = 0;
    }
    if (i2cBufferReadTemp != pBufferI2CWrite) // Buffer not empty
    {
        pBufferI2CRead = i2cBufferReadTemp;
        i2cData = gBufferI2C[pBufferI2CRead];
    }
    return i2cData;
}


/*
#pragma vector = 21
__interrupt void i2cIntHandler(void)
{
    I2C_ClearITPendingBit(I2C_ITPENDINGBIT_RXNOTEMPTY);
    gBufferI2C[gBufferI2C[INDEX_LENGTH]] =  I2C_ReceiveData();
    gBufferI2C[INDEX_LENGTH];
    gReceiveByteI2C--;
}
*/

/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
