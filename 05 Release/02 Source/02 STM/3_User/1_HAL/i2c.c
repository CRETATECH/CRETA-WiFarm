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
    uint16_t result = 0x0004;
    uint8_t tmpccrh = 0;

    CLK->PCKENR1 |= (uint8_t)((uint8_t)1 << ((uint8_t)0x00 & (uint8_t)0x0F));

    uint8_t Input_Clock = 0x00;
    Input_Clock = CLK_GetClockFreq()/1000000;
    // I2C->CR1 |= I2C_CR1_PE;
    I2C_Init(pSpeed, pOwnAddr, I2C_DUTYCYCLE_2,
             I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);

    I2C->FREQR &= (uint8_t)(~I2C_FREQR_FREQ);
    I2C->FREQR |= Input_Clock;
    /* Disable I2C to configure TRISER */
    I2C->CR1 &= (uint8_t)(~I2C_CR1_PE);

    /* Clear CCRH & CCRL */
    I2C->CCRH &= (uint8_t)(~(I2C_CCRH_FS | I2C_CCRH_DUTY | I2C_CCRH_CCR));
    I2C->CCRL &= (uint8_t)(~I2C_CCRL_CCR);

    if(pSpeed > (uint32_t)100000) /* Fast mode */
    {
        tmpccrh = I2C_CCRH_FS;
        result = (uint16_t) ((Input_Clock * 1000000) / (pSpeed * 3));
        /* Verify and correct CCR value if below minimum value */
        if (result < (uint16_t)0x01)
        {
            /* Set the minimum allowed value */
            result = (uint16_t)0x0001;
        }
    }
    else /* Standard mode */
    {
        /* Calculate standard mode speed */
        result = (uint16_t)((Input_Clock * 1000000) / (pSpeed << (uint8_t)1));

        /* Verify and correct CCR value if below minimum value */
        if (result < (uint16_t)0x0004)
        {
          /* Set the minimum allowed value */
          result = (uint16_t)0x0004;
        }

        /* Set Maximum Rise Time: 1000ns max in Standard Mode
        = [1000ns/(1/Input_Clock.10e6)]+1
        = Input_Clock+1 */
        I2C->TRISER = (uint8_t)(Input_Clock + (uint8_t)1);
    }
    /* Write CCR with new calculated value */
    I2C->CCRL = (uint8_t)result;
    I2C->CCRH = (uint8_t)((uint8_t)((uint8_t)(result >> 8) & I2C_CCRH_CCR) | tmpccrh);

    /* Enable I2C */
    I2C->CR1 |= I2C_CR1_PE;

    I2C->CR2 &= (uint8_t)(~I2C_CR2_POS);

    I2C->OARL = (uint8_t)(pOwnAddr);
    I2C->OARH = (uint8_t)((uint8_t)(0x00 | I2C_OARH_ADDCONF) |
                   (uint8_t)((pOwnAddr & (uint16_t)0x0300) >> (uint8_t)7));

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
    I2C->CR2 |= I2C_CR2_START;
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT))
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    I2C->DR = (uint8_t)pAddr;
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
    I2C->CR2 |= I2C_CR2_STOP;
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
    I2C->CR2 |= I2C_CR2_START;
    while (I2C_GetLastEvent() != I2C_EVENT_MASTER_MODE_SELECT)
    {
        vTimeOut--;
        if(0 == vTimeOut)
            return EXIT_FAILURE;
    }
    I2C->DR = (uint8_t)pAddr | 0x01;
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
        *pBuff =  ((uint8_t)I2C->DR);
        pBuff++;
        pLength--;
    }
    I2C->CR2 |= I2C_CR2_STOP;
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
    I2C->DR = pData;
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
