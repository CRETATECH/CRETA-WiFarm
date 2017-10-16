/*******************************************************************************
  * @filename   : sht1x.c
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
#include "sht1x.h"
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
static void shtStart();
static void shtCmd(uint8_t cmd);
static uint8_t shtCheckACK();
static uint8_t shtWaitConvert();
static uint16_t shtReadRaw();

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief To init SHT10
 * @param None
 */
void shtInit(void)
{
    // Just delay to power up
    uint16_t _sht_startup_delay = 0xFFFF;
    while(_sht_startup_delay --);
}
/**
 * @brief To read temperature
 * @param *temp
 * @ret
 *      EXIT_SUCCESS
 *      EXIT_FAILURE
 */
uint8_t shtReadTemp(float *temp)
{
    const float d1 = -39.6;
    const float d2 = 0.01;
    float ftemp;

    shtStart();
    shtCmd(SHT_CMD_TEMP);
    if(shtCheckACK() == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(EXIT_FAILURE == shtWaitConvert())
        return EXIT_FAILURE;
    ftemp = shtReadRaw();
    ftemp = d1 + d2*ftemp;
    *temp = ftemp;
    return EXIT_SUCCESS;
}
/**
 * @brief To read humid
 * @param *temp
 * @ret
 *      EXIT_SUCCESS
 *      EXIT_FAILURE
 */
uint8_t shtReadHumid(float *humid)
{
    const float c1 = -4;
    const float c2 = 0.0405;
    const float c3 = -2.8E-6;
    const float t1 = 0.01;
    const float t2 = 0.00008;
    float rhumid;
    float ftemp;
    float fhumidl;
    float fhumid;
    shtStart();
    shtCmd(SHT_CMD_HUMID);
    if(shtCheckACK() == EXIT_FAILURE)
        return EXIT_FAILURE;
    if(EXIT_FAILURE == shtWaitConvert())
        return EXIT_FAILURE;
    rhumid = (shtReadRaw());
    fhumidl = c1 + c2*rhumid + c3*rhumid*rhumid;
    shtReadTemp(&ftemp);
    fhumid = (ftemp - 25)*(t1 + t2*rhumid)+fhumidl;
    *humid = fhumid;
    return EXIT_SUCCESS;
}

/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/
/**
 * @brief To send start signal
 * @param None
 */
static void shtStart(void)
{
    GPIO_Init(SHT_PORT, SHT_SCK_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(SHT_PORT, SHT_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
    GPIO_WriteHigh(SHT_PORT, SHT_SDA_PIN);
    GPIO_WriteHigh(SHT_PORT, SHT_SCK_PIN);
    GPIO_WriteLow(SHT_PORT, SHT_SDA_PIN);
    GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
    GPIO_WriteHigh(SHT_PORT, SHT_SCK_PIN);
    GPIO_WriteHigh(SHT_PORT,SHT_SDA_PIN);
    GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
}

/**
 * @brief To send a command
 * @param pCmd
 */
static void shtCmd(uint8_t pCmd)
{
    uint8_t _sht_cmd_mask = 0x80;
    int i;
    for(i=0; i<8; i++)
    {
        if((pCmd & _sht_cmd_mask) != 0)
            GPIO_WriteHigh(SHT_PORT, SHT_SDA_PIN);
        else
            GPIO_WriteLow(SHT_PORT, SHT_SDA_PIN);
        GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
        GPIO_WriteHigh(SHT_PORT, SHT_SCK_PIN);
        GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
        _sht_cmd_mask = _sht_cmd_mask >> 1;
    }
}

/**
 * @brief To check SHT ACK
 * @param None
 * @ret
 *      EXIT_SUCCESS
 *      EXIT_FAILURE
 */
static uint8_t shtCheckACK(void)
{
    GPIO_Init(SHT_PORT, SHT_SDA_PIN, GPIO_MODE_IN_FL_NO_IT);
    if(GPIO_ReadInputPin(SHT_PORT, SHT_SDA_PIN) != RESET)
        return EXIT_FAILURE;
    GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
    GPIO_WriteHigh(SHT_PORT, SHT_SCK_PIN);
    GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
    if(GPIO_ReadInputPin(SHT_PORT, SHT_SDA_PIN) == RESET)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
/**
 * @brief To wait for SHT convertion
 * @param None
 * @ret
 *      EXIT_SUCCESS
 *      EXIT_FAILURE
 */
static uint8_t shtWaitConvert()
{
    uint16_t time_out = 0xFFFF;
    GPIO_Init(SHT_PORT, SHT_SDA_PIN, GPIO_MODE_IN_FL_NO_IT);
    while((GPIO_ReadInputPin(SHT_PORT, SHT_SDA_PIN) != RESET)||(time_out == 0))
        time_out--;
    if(0 == time_out)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
/**
 * @brief To read raw data
 * @param None
 * @ret   Raw data
 */
static uint16_t shtReadRaw(void)
{
    uint16_t _sht_data_raw = 0x0000;
    int i;
    for(i=0; i<8; i++)
    {
        GPIO_WriteLow(SHT_PORT,SHT_SCK_PIN);
        GPIO_WriteHigh(SHT_PORT,SHT_SCK_PIN);
        _sht_data_raw = _sht_data_raw << 1;
        if(GPIO_ReadInputPin(SHT_PORT, SHT_SDA_PIN) != RESET)
            _sht_data_raw |= 0x0001;
        GPIO_WriteLow(SHT_PORT,SHT_SCK_PIN);
    }
    GPIO_Init(SHT_PORT, SHT_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_WriteLow(SHT_PORT,SHT_SDA_PIN);
    GPIO_WriteLow(SHT_PORT,SHT_SCK_PIN);
    GPIO_WriteHigh(SHT_PORT,SHT_SCK_PIN);
    GPIO_WriteLow(SHT_PORT,SHT_SCK_PIN);
    GPIO_Init(SHT_PORT, SHT_SDA_PIN, GPIO_MODE_IN_FL_NO_IT);
    for(i=0; i<8; i++)
    {
        GPIO_WriteLow(SHT_PORT,SHT_SCK_PIN);
        GPIO_WriteHigh(SHT_PORT,SHT_SCK_PIN);
        _sht_data_raw = _sht_data_raw << 1;
        if(GPIO_ReadInputPin(SHT_PORT, SHT_SDA_PIN) != RESET)
            _sht_data_raw |= 0x0001;
        GPIO_WriteLow(SHT_PORT,SHT_SCK_PIN);
    }
    GPIO_Init(SHT_PORT, SHT_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_WriteHigh(SHT_PORT,SHT_SDA_PIN);
    GPIO_WriteLow(SHT_PORT,SHT_SCK_PIN);
    GPIO_WriteHigh(SHT_PORT,SHT_SCK_PIN);
    GPIO_WriteLow(SHT_PORT,SHT_SCK_PIN);
    return _sht_data_raw;
}

/******************************************************************************/
/* END OF FILE */
/******************************************************************************/
