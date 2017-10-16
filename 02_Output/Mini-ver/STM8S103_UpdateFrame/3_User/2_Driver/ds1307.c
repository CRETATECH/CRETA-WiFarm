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
 * @brief   To set DS1307 time and run
 * @param   None
 */
int ds1307SetTime(uint8_t pYear, uint8_t pMonth, uint8_t pDay, uint8_t pHour, uint8_t pMin)
{
    pYear = INT2BCD(pYear);
    pMonth = INT2BCD(pMonth);
    pDay = INT2BCD(pDay);
    pHour = INT2BCD(pHour);
    pMin = INT2BCD(pMin);
    //TODO: Release I2C code.
    if(EXIT_SUCCESS != i2cBeginTransmission(0xD0)) return EXIT_FAILURE;
    //<! Call register address
    if(EXIT_SUCCESS != i2cWrite(0x00)) return EXIT_FAILURE;
    //<! Second
     if(EXIT_SUCCESS != i2cWrite(0x00)) return EXIT_FAILURE;
    //<! Minute
    if(EXIT_SUCCESS != i2cWrite(pMin)) return EXIT_FAILURE;
    if(EXIT_SUCCESS != i2cWrite(pHour)) return EXIT_FAILURE;
    if(EXIT_SUCCESS != i2cWrite(pDay)) return EXIT_FAILURE;
    if(EXIT_SUCCESS != i2cWrite(2)) return EXIT_FAILURE;
    if(EXIT_SUCCESS != i2cWrite(pMonth)) return EXIT_FAILURE;
    if(EXIT_SUCCESS != i2cWrite(pYear)) return EXIT_FAILURE;
    i2cEndTransmission();
    return EXIT_SUCCESS;
}


/**
 * @brief   To read time value from DS1307
 * @param   pRegAdd
 */
int ds1307Read(uint8_t pRegAdd, uint8_t *pData)
{
    uint8_t data;
    if(EXIT_SUCCESS != i2cBeginTransmission(0xD0)) return EXIT_FAILURE;
    if(EXIT_SUCCESS != i2cWrite(pRegAdd)) return EXIT_FAILURE;
    I2C_AcknowledgeConfig(I2C_ACK_NONE);
    if(EXIT_SUCCESS != i2cRequestFrom(0xD0, &data, 1)) return EXIT_FAILURE;
    while(I2C->CR2 & I2C_CR2_STOP);
    I2C_AcknowledgeConfig(I2C_ACK_CURR);
    *pData = BCD2INT(data);
    return EXIT_SUCCESS;
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
