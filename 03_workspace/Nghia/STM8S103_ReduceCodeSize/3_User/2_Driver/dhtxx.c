/*******************************************************************************
  * @filename   : dhtxx.c
  * @author     : HgN
  * @last update: August 17th, 2017
  */
/******************************************************************************/

/******************************************************************************/
/* INCLUDE */
/******************************************************************************/
#include "dhtxx.h"
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
volatile uint32_t gDhtUsTiming = 0;
/******************************************************************************/
/* LOCAL FUNCTION PROTOTYPES */
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTIONS */
/******************************************************************************/
/**
 * @brief   To init DHTxx sensor
 * @param   None
 */
void dhtInit(void)
{
    gpioPinMode(DHT_DATA_PORT, DHT_DATA_PIN, GPIO_OUTPUT);
    gpioWritePin(DHT_DATA_PORT, DHT_DATA_PIN, GPIO_HIGH);
}
/**
 * @brief   To generate START
 * @param   None
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int dhtStart(void)
{
    uint16_t vTimeout = 0xFFFF;
    //<! Reset data pin 0 for over 800us
    gpioPinMode(DHT_DATA_PORT, DHT_DATA_PIN, GPIO_OUTPUT);
    gpioWritePin(DHT_DATA_PORT, DHT_DATA_PIN, GPIO_LOW);
    gDhtUsTiming = 20000;
    TIM4->IER |= (uint8_t)TIM4_IT_UPDATE;
    while(0 != gDhtUsTiming);
    //<! Set data pin 1, pin mode Input
    gpioWritePin(DHT_DATA_PORT, DHT_DATA_PIN, GPIO_HIGH);
    gpioPinMode(DHT_DATA_PORT, DHT_DATA_PIN, GPIO_INPUT);
    //<! Wait until dht reset data pin 0
    while(GPIO_LOW != gpioReadPin(DHT_DATA_PORT, DHT_DATA_PIN))
    {
        vTimeout--;
        if(vTimeout == 0)
            return 5;
            // return EXIT_FAILURE;
    }
    //<! Wait until dht set to 1
    while(GPIO_LOW == gpioReadPin(DHT_DATA_PORT, DHT_DATA_PIN))
    {
        vTimeout--;
        if(vTimeout == 0)
            return 6;
            // return EXIT_FAILURE;
    }
    //<! Wait until dht reset 0 (Start transmission)
    while(GPIO_HIGH == gpioReadPin(DHT_DATA_PORT, DHT_DATA_PIN))
    {
        vTimeout--;
        if(vTimeout == 0)
            return 7;
            // return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
/**
 * @brief   To read humid and temperature
 * @param   *pHumid
 * @param   *pTemp
 * @retval
 *          EXIT_SUCCESS
 *          EXIT_FAILURE
 */
int dhtRead(float* pHumid, float* pTemp)
{
    int lowTime = 0;
    int highTime = 0;
    if(EXIT_SUCCESS != dhtStart())
        return EXIT_FAILURE;
    uint8_t byte[5] = {0, 0, 0, 0, 0};
    int i, j;
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 8; j++)
        {
            byte[i] = byte[i] << 1;
            while(GPIO_LOW == gpioReadPin(DHT_DATA_PORT, DHT_DATA_PIN))
                lowTime++;
            while(GPIO_HIGH == gpioReadPin(DHT_DATA_PORT, DHT_DATA_PIN))
                highTime++;
            if(lowTime < highTime)
                byte[i] |= 0x01;
            else
                byte[i] |= 0x00;
            lowTime = 0;
            highTime = 0;
        }
    }
    if(byte[4] != (uint8_t)(byte[0] + byte[1] + byte[2] + byte[3]))
        return EXIT_FAILURE;
    *pHumid = (float)byte[0];
    *pTemp = (float)byte[2];
    return EXIT_SUCCESS;
}
/******************************************************************************/
/* LOCAL FUNCTIONS */
/******************************************************************************/
#pragma vector = 25
__interrupt void dhtUSCounter(void)
{
    //<! CLear pending bit
    TIM4->SR1 = (uint8_t)(~TIM4_IT_UPDATE);
    if(0 != gDhtUsTiming)
        gDhtUsTiming--;
    else
        TIM4->IER &= (uint8_t)(~TIM4_IT_UPDATE);
}
/******************************************************************************/
/* END OF FILE */
/******************************************************************************/