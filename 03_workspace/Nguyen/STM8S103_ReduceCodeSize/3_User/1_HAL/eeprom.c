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

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief To init STM8S internal EEPROM
 * @param None
 */
void eepromInit()
{
    FLASH->DUKR = FLASH_RASS_KEY2;
    FLASH->DUKR = FLASH_RASS_KEY1;
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
        *(PointerAttr uint8_t*) (MemoryAddressCast)vAddr = (uint8_t)0x00;
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
void eepromWrite(uint32_t pAddr, uint8_t *pData, uint8_t pLength)
{
    /*
    if(EXIT_FAILURE == eepromAddrCheck(pAddr))
        return EXIT_FAILURE;
    */
    int i;
    for(i = 0; i < pLength; i++)
    {
        *(PointerAttr uint8_t*) (MemoryAddressCast)(pAddr+i) = *pData;
        pData++;
    }
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
void eepromRead(uint32_t pAddr, uint8_t *pBuff, uint8_t pLength)
{
    int i;
    for(i = 0; i < pLength; i++)
    {
        *pBuff = *(PointerAttr uint8_t *) (MemoryAddressCast)(pAddr+i);
        pBuff++;
    }
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
