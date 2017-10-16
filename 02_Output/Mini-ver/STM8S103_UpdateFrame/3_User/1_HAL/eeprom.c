/*******************************************************************************
  * @filename   : eeprom.c
  * @author     : HgN
  * @last update: July 20th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "eeprom.h"
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

/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/
int eepromAddrCheck(uint32_t pAddr);
/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief To init STM8S internal EEPROM
 * @param None
 */
void eepromInit()
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
}

/**
 * @brief To erase internal EEPROM
 * @param None
 */
void eepromClear()
{
    uint32_t vAddr;
    for(vAddr = EEPROM_START_ADDR; vAddr < (EEPROM_END_ADDR + 1); vAddr++)
    {
        FLASH_EraseByte(vAddr);
    }
}

/**
 * @brief To write data to EEPROM
 * @param pAddr
 * @param *pData
 * @param pLength
 * @ret
 *      EXIT_SUCCESS
 *      EXIT_FAILURE
 */
int eepromWrite(uint32_t pAddr, uint8_t *pData, uint8_t pLength)
{
    if(EXIT_FAILURE == eepromAddrCheck(pAddr))
        return EXIT_FAILURE;
    int i;
    for(i = 0; i < pLength; i++)
    {
        FLASH_ProgramByte((pAddr+i), *pData);
        pData++;
    }
    return EXIT_SUCCESS;

}

/**
 * @brief To read from EEPROM
 * @param pAddr
 * @param *pBuff
 * @param pLength
 * @ret
 *      EXIT_SUCCESS
 *      EXIT_FAILURE
 */
int eepromRead(uint32_t pAddr, uint8_t *pBuff, uint8_t pLength)
{
    if(EXIT_FAILURE == eepromAddrCheck(pAddr))
        return EXIT_FAILURE;
    int i;
    for(i = 0; i < pLength; i++)
    {
        *pBuff = FLASH_ReadByte(pAddr+i);
        pBuff++;
    }
    return EXIT_SUCCESS;
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/
int eepromAddrCheck(uint32_t pAddr)
{
    if((true == (pAddr >= EEPROM_START_ADDR) && \
                (pAddr <= EEPROM_END_ADDR)))
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
