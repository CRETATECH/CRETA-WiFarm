/* Include */
#include "sht1x.h"
#include <stdbool.h>

/* Local variables */

/* Private Function Protocol */
void shtStart();
void shtCmd(uint8_t cmd);
uint8_t shtCheckACK();
uint8_t shtWaitConvert();
uint16_t shtReadRaw();

/* Public Function */
void shtInit()
{
    // Just delay to power up
    uint16_t _sht_startup_delay = 0xFFFF;
    while(_sht_startup_delay --);
}

uint8_t shtReadTemp(float *temp)
{
    const float d1 = -39.6;
    const float d2 = 0.01;
    float ftemp;

    shtStart();
    shtCmd(SHT_CMD_TEMP);
    if(shtCheckACK() == SYS_ERR)
        return SYS_ERR;
    if(SYS_ERR == shtWaitConvert())
        return SYS_ERR;
    ftemp = shtReadRaw();
    ftemp = d1 + d2*ftemp;
    *temp = ftemp;
    return SYS_OK;
}

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
    if(shtCheckACK() == SYS_ERR)
        return SYS_ERR;
    if(SYS_ERR == shtWaitConvert())
        return SYS_ERR;
    rhumid = (shtReadRaw());
    fhumidl = c1 + c2*rhumid + c3*rhumid*rhumid;
    shtReadTemp(&ftemp);
    fhumid = (ftemp - 25)*(t1 + t2*rhumid)+fhumidl;
    *humid = fhumid;
    return SYS_OK;
}

/* Local Function */

void shtStart()
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

void shtCmd(uint8_t cmd)
{
    uint8_t _sht_cmd_mask = 0x80;
    int i;
    for(i=0; i<8; i++)
    {
        if((cmd & _sht_cmd_mask) != 0)
            GPIO_WriteHigh(SHT_PORT, SHT_SDA_PIN);
        else
            GPIO_WriteLow(SHT_PORT, SHT_SDA_PIN);
        GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
        GPIO_WriteHigh(SHT_PORT, SHT_SCK_PIN);
        GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
        _sht_cmd_mask = _sht_cmd_mask >> 1;
    }
}

uint8_t shtCheckACK()
{
    GPIO_Init(SHT_PORT, SHT_SDA_PIN, GPIO_MODE_IN_FL_NO_IT);
    if(GPIO_ReadInputPin(SHT_PORT, SHT_SDA_PIN) != RESET)
        return SYS_ERR;
    GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
    GPIO_WriteHigh(SHT_PORT, SHT_SCK_PIN);
    GPIO_WriteLow(SHT_PORT, SHT_SCK_PIN);
    if(GPIO_ReadInputPin(SHT_PORT, SHT_SDA_PIN) == RESET)
        return SYS_ERR;
    return SYS_OK;
}

uint8_t shtWaitConvert()
{
    uint16_t time_out = 0xFFFF;
    GPIO_Init(SHT_PORT, SHT_SDA_PIN, GPIO_MODE_IN_FL_NO_IT);
    while((GPIO_ReadInputPin(SHT_PORT, SHT_SDA_PIN) != RESET)|(time_out == 0))
        time_out--;
    if(0 == time_out)
        return SYS_ERR;
    else
        return SYS_OK;
}

uint16_t shtReadRaw()
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
