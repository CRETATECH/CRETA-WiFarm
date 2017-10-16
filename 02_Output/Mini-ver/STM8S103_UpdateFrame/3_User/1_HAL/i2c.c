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
#include "gpio.h"
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
/*
    gpioPinMode(GPIO_PORTB, GPIO_PIN_4, GPIO_OUTPUT);
    int i;
    int time;
    for(i = 0; i < 10; i++)
    {
        gpioWritePin(GPIO_PORTB, GPIO_PIN_4, GPIO_HIGH);
        time = 0xFF;
        while(time--);
        gpioWritePin(GPIO_PORTB, GPIO_PIN_4, GPIO_LOW);
        time = 0xFF;
        while(time--);
    }
    GPIO_DeInit(GPIOB);
*/
//TODO: Remove GPIO_DeInit();
    I2C_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
    uint8_t Input_Clock = 0x00;
    Input_Clock = CLK_GetClockFreq()/1000000;
    I2C_Cmd(ENABLE);
    I2C_Init(pSpeed, pOwnAddr, I2C_DUTYCYCLE_2,
             I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);
    //I2C_StretchClockCmd(ENABLE);
    //I2C_ITConfig(I2C_IT_BUF, ENABLE);
}
/**
 * @brief To begin a transmisstion
 * @param pAddr
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int i2cBeginTransmission(uint8_t pAddr)
{
    uint32_t vTimeOut = 0xFF;
    while(I2C_GetFlagStatus( I2C_FLAG_BUSBUSY)==SET)
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    I2C_GenerateSTART(ENABLE);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    I2C_Send7bitAddress(pAddr, I2C_DIRECTION_TX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
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
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int i2cRequestFrom(uint8_t pAddr, uint8_t *pBuff, uint8_t pLength)
{
    uint32_t vTimeOut = 0xFF;
    I2C_GenerateSTART(ENABLE);
    while (I2C_GetLastEvent() != I2C_EVENT_MASTER_MODE_SELECT)
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    I2C_Send7bitAddress(pAddr, I2C_DIRECTION_RX);
    while (I2C_GetLastEvent() != I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    I2C->SR1;		I2C->SR3;
    while (pLength != 0)
    {
        vTimeOut = 0xFF;
        while (I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET)
        {
            vTimeOut--;
            if(0 == vTimeOut)
                return EXIT_FAILURE;
        }
        *pBuff =  I2C_ReceiveData();
        pBuff++;
        pLength--;
    }
    I2C_GenerateSTOP(ENABLE);
    I2C->SR1;		I2C->SR3;
    return EXIT_SUCCESS;
} // TODO: no buffer overflow solution yet
/**
 * @brief   To send 1 byte data to i2c line
 * @param   pData
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int i2cWrite(uint8_t pData)
{
    uint32_t vTimeOut = 0xFF;
    I2C_SendData(pData);
    while(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
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
