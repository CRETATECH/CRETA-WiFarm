/**
  ******************************************************************************
  * SOURCE FILE FOR MODLULE REALTIME DS1307
  * File        ds1307.C
  * Author      HgN
  * Version     V1.0.0
  * Date        May 27th, 2017
  * Last update May 27th, 2017
  ******************************************************************************
  */

#include "ds1307.h"

/* Private Functions --------------------------------------------------------- */
uint8_t BCD2BIN(uint8_t dataBCD);
uint8_t INT2BCD(uint8_t dataINT);

/* Functions ----------------------------------------------------------------- */
/* BCD2INT
 * Change the given 8-bit BCD number to binary format
 * ==ARGS==
 * __dataBCD: 8-bit BCD number
 */
uint8_t BCD2INT(uint8_t dataBCD)
{
    uint8_t dataINT = (dataBCD>>4)*10+(dataBCD&(0x0F));
    return dataINT;
}


/* INT2BCD
 * Change the given 8-bit binary number to BCD format
 * ==ARGS==
 * __dataINT: 8-bit binary number
 */
uint8_t INT2BCD(uint8_t dataINT)
{
    uint8_t dataBCD = ((dataINT/10)<<4)|(dataINT%10);
    return dataBCD;
}


/* DS1307 INIT
 * Set time for module ds1307 and let it run
 * ==ARGS==
 * __pYear, pMonth, pDay, pHour, pMin: if you set it larger than its range (ex: 13th month),
 * ds1307 will automatic set to maximum possible number.
 */
void ds1307Init(uint8_t pYear, uint8_t pMonth, uint8_t pDay, uint8_t pHour, uint8_t pMin)
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


/* DS1307 READ
 * Read ds1307 reg (time)
 * ==ARGS==
 * __pRegAdd: DS1307_REG_SEC, DS1307_REG_MIN, DS1307_REG_HOUR, DS1307_REG_DAY,
 * DS1307_REG_MONTH, DS1307_REG_YEAR
 * ==RETURN==
 * __Reg value in binary format.
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