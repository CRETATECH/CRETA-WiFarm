
#include "DS1037.h" 

/**
  * Function Name: writeDS1307
  * Description:   send data via I2C to DS1307_REG_YEAR
  * Input:         data
  * Output:        data
  * Return:        1 if sucess // 0 if fail
  */
int writeDS1307(uint8_t data)
{
    if((i2c_WriteByte(DS1307_ADDRESS, DS1307_REG_YEAR, data)) == EXIT_FAILURE)
    {
        i2cEndTransmission();
        return EXIT_FAILURE;
    }
    else
        i2cEndTransmission();
        return EXIT_SUCCESS;
    
}
/**
  * Function Name: readDS1307
  * Description:   read data via I2C to DS1307_REG_YEAR
  * Input:         data
  * Output:        data
  * Return:        1 if sucess // 0 if fail
  */
int readDS1307(uint8_t data)
{   uint8_t pBuffer = 0;
    if((i2c_ReadByte(DS1307_ADDRESS, DS1307_REG_YEAR, &pBuffer)) == EXIT_FAILURE)
    {
        i2cEndTransmission();
        return EXIT_FAILURE;  
    }
    else
    {
        if (pBuffer == data)
        {
            i2cEndTransmission();
            return EXIT_SUCCESS;
        }
        else
            i2cEndTransmission();
            return EXIT_FAILURE;
    }
}


//int i2c_ReadByte(uint8_t SLAVE_ADDRESS, uint8_t ReadAdress, uint8_t *pBuffer);
