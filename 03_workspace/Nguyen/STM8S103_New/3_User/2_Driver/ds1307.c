/*******************************************************************************
  * @filename   : ds1307.c
  * @author     : HgN
  * @last update: July 20th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "ds1307.h"
#include <stdbool.h>
#include <stdlib.h>

/******************************************************************************/
/* LOCAL TYPEDEFS */
/******************************************************************************/

/******************************************************************************/
/* LOCAL DEFINES */
/******************************************************************************/

/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/
uint8_t BCD2INT(uint8_t dataBCD);
uint8_t INT2BCD(uint8_t dataINT);

/******************************************************************************/
/* PUBLIC VARIABLES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief To set DS1307 time and run
 * @param
 */
void ds1307SetTime(uint8_t pYear, uint8_t pMonth, uint8_t pDay, uint8_t pHour, uint8_t pMin)
{
    pYear = INT2BCD(pYear);
    pMonth = INT2BCD(pMonth);
    pDay = INT2BCD(pDay);
    pHour = INT2BCD(pHour);
    pMin = INT2BCD(pMin);
    while(I2C_GetFlagStatus( I2C_FLAG_BUSBUSY)==SET);
    I2C_GenerateSTART(ENABLE);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(0xD0,I2C_DIRECTION_TX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData( 0x00);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData( 0);
    while(!I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED));
    I2C_SendData( pMin);
    while(!I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED));
    I2C_SendData( pHour);
    while(!I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED));
    I2C_SendData( pDay);
    while(!I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED));
    I2C_SendData( 2);
    while(!I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED));
    I2C_SendData( pMonth);
    while(!I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED));
    I2C_SendData( pYear);
    while(!I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED));
    I2C_GenerateSTOP(ENABLE);
    I2C->SR1;		I2C->SR3;
}


/**
 * @brief   To read time value from DS1307
 * @param   pRegAdd
 */
uint8_t ds1307Read(uint8_t pRegAdd)
{
    uint8_t data;
    I2C_GenerateSTART(ENABLE);
    // while(I2C_GetFlagStatus(I2C_FLAG_STARTDETECTION)==SET);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT));
    //send ds1307 address
    I2C_Send7bitAddress(0xD0,I2C_DIRECTION_TX);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    //send ds1307's register address to read
    I2C_SendData(pRegAdd);
    while(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET);
    I2C_GenerateSTART( ENABLE);
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(0xD0,I2C_DIRECTION_RX);
    while(I2C_GetFlagStatus( I2C_FLAG_ADDRESSSENTMATCHED) == RESET);
    I2C_AcknowledgeConfig(I2C_ACK_NONE);
    I2C->SR1;		I2C->SR3;
    I2C_GenerateSTOP(ENABLE);
    while (I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET);
    data=I2C_ReceiveData();
    while(I2C->CR2 & I2C_CR2_STOP);
    I2C_AcknowledgeConfig( I2C_ACK_CURR);
    data = BCD2INT(data);
    return data;
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/
/**
 * @brief   To convert BCD number to integer
 * @param   dataBCD
 * @retval  dataINT
 */
uint8_t BCD2INT(uint8_t dataBCD)
{
    uint8_t dataINT = (dataBCD>>4)*10+(dataBCD&(0x0F));
    return dataINT;
}


/**
 * @brief   To conver integer number to BCD
 * @param   dataINT
 * @retval  dataBCD
 */
uint8_t INT2BCD(uint8_t dataINT)
{
    uint8_t dataBCD = ((dataINT/10)<<4)|(dataINT%10);
    return dataBCD;
}

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
