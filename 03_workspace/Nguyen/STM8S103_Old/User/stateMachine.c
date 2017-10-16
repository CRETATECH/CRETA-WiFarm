/**
  ******************************************************************************
  * SOURCE FILE OF STM8 STATE MACHINE
  * File        stateMachine.c
  * Author      HgN
  * Version     V1.0.0
  * Date        May   25th, 2017
  * Last update May   25th, 2017
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "stateMachine.h"

/* Global variables ----------------------------------------------------------*/
static uint8_t gState = STATE_IDLE;

uint8_t frameReceiveBuffer[12];
uint8_t frameTransmitBuffer[13] = {'*',0,0,0,0,0,0,0,0,0,0,0,'#'};
/* Private Functions ----------------------------------------------------------*/
void stateUART();
void stateIdle();
void stateSensor();
bool cmdGet();
void cmdDo();
void cmdDoDim();
void cmdDoStatus();
void cmdDoStatusOne();
void cmdFrameClear();

/* Function ------------------------------------------------------------------*/

/* STATE UPDATE
 * Check all event and update the value of globel variable gState
 * __STATE_UART: When reveive a command sent via UART
 * __STATE_IDLE: When there's nothing to do
 */
void stateUpdate()
{
    static uint32_t timeSensor = 0;
	if(uartAvailable() != 0)
	{
		if (uartRead() == '*')
		{
			frameReceiveBuffer[0] = '*'; 
			gState = STATE_UART;
		}
	}
    else if((timerGetSecond() - timeSensor) > SENSOR_GAP)
    {
        gState = STATE_SENSOR;
        timeSensor = timerGetSecond();
    }
	else
	{
		gState = STATE_IDLE;
	}
}


/* STATE MACHINE RUNNING
 * Check value of gState and run into the state
 */
void stateMachine()
{
	switch(gState)
	{
		case STATE_UART:
			stateUART();
		break;
		case STATE_IDLE:
			stateIdle();
		break;
        case STATE_SENSOR:
            stateSensor();
		default:
		break;
	}
}


/* STATE UART
 * Get the command from UART buffer, check if it's ok, then do the request.
 */
void stateUART()
{
	bool isCmdOK;
	isCmdOK = cmdGet();
	if (isCmdOK == true)
	{
		cmdDo();
	}
	cmdFrameClear();
	gState = STATE_IDLE;
}

/* STATE SENSOR
 * Read humid sensor, save to EEPROM and send to esp via uart
 */
void stateSensor()
{
    // static uint16_t flash_address = FLASH_MIN_ADDR;
    float fhumid;
    uint16_t humid;
    if(SYS_OK == shtReadHumid(&fhumid))
    {
	    humid = (uint16_t)(fhumid*100);
	    frameTransmitBuffer[0] = '*';
	    frameTransmitBuffer[1] = FUNCTION_DATA;
	    frameTransmitBuffer[2] = DEVICE_HUMID;
	    frameTransmitBuffer[3] = 0x00;
	    frameTransmitBuffer[10] = (uint8_t)(humid>>8);
	    frameTransmitBuffer[11] = (uint8_t)humid;
	    frameTransmitBuffer[12] = '#';
	    uartWrite(frameTransmitBuffer, 13);
	    // FLASH_ProgramByte(flash_address, (uint8_t)humid);
	    // flash_address++;
	    // if(flash_address > FLASH_MAX_ADDR)
	    // 	flash_address = FLASH_MIN_ADDR;
	}
}

/* STATE IDLE
 * Do nothing
 */
void stateIdle()
{
	// Do nothing
}


/* CMD GET
 * Get the command from UART buffer to frameReceive buffer and check it
 * ==RETURN==
 * __true: if the frame is ok
 * __false: if the frame is not right
 */
bool cmdGet()
{
	bool isCmdOK = false;
	bool frameGetDone = false;
	uint32_t frameTimeOut = 0x0000;
	uint8_t index = 1;
	while (frameGetDone == false)
	{
		if(uartAvailable() != 0)
		{
			frameReceiveBuffer[index] = uartRead();
			index++;
		}
		if(index > 11) // framReceiveBuffer full but no '#' found
		{
			frameGetDone = true;
		}
		else if(frameTimeOut > 0xFFFFFFF0) // Time out
		{
			frameGetDone = true;
		}
		else
		{
			frameTimeOut++;
		}
	} //TODO: Kiem tra lai viec doc frame
	if (frameReceiveBuffer[11] == '#')
	{
		isCmdOK = true;
	}
	return isCmdOK;
}


/* CMD Do
 * Do the request of given frame
 */
void cmdDo()
{
	uint8_t function = frameReceiveBuffer[1];
	switch(function)
	{
		case FUNCTION_DIM:
			cmdDoDim();
		break;
		case FUNCTION_STATUS:
			cmdDoStatus();
		break;
		default:
		break;
	}
}


/* CMD DO DIM
 * Do the dim task, if the device is an ON/OFF device, it'll be set ON if the received value >50
 */
void cmdDoDim()
{
	uint8_t level = frameReceiveBuffer[10];
	uint8_t device = frameReceiveBuffer[2];
	uint8_t devicePin;
	uint16_t devicePort;
	switch(device)
	{
		case DEVICE_ONOFF_1:
			devicePin = DEVICE_1_PIN;
		break;
		case DEVICE_ONOFF_2:
			devicePin = DEVICE_2_PIN;
		break;
		case DEVICE_ONOFF_3:
			devicePin = DEVICE_3_PIN;
		break;
		default:
			devicePin = DEVICE_NONE;
		break;
	}
	switch(device)
	{
		case DEVICE_ONOFF_1:
			devicePort = DEVICE_1_PORT;
		break;
		case DEVICE_ONOFF_2:
			devicePort = DEVICE_2_PORT;
		break;
		case DEVICE_ONOFF_3:
			devicePort = DEVICE_3_PORT;
		break;
		default:
			devicePort = DEVICE_NONE;
		break;
	}
	if (devicePin != DEVICE_NONE)
	{
		if (level > 50)
		{
			gpioWritePin(devicePort, devicePin, HIGH);
		}
		else
		{
			gpioWritePin(devicePort, devicePin, LOW);
		}
	}
	cmdDoStatusOne();
}


/* CMD DO STATUS
 * Send back the ouput device status via UART
 */
void cmdDoStatus()
{
	frameTransmitBuffer[0] = '*';
	frameTransmitBuffer[1] = FUNCTION_STATUS;
	frameTransmitBuffer[2] = DEVICE_ONOFF_1;
	frameTransmitBuffer[3] = 0x01;
	frameTransmitBuffer[11] = deviceGetStatus(DEVICE_ONOFF_1);
	frameTransmitBuffer[12] = '#';
	uartWrite(frameTransmitBuffer, 13);
	frameTransmitBuffer[0] = '*';
	frameTransmitBuffer[1] = FUNCTION_STATUS;
	frameTransmitBuffer[2] = DEVICE_ONOFF_2;
	frameTransmitBuffer[3] = 0x01;
	frameTransmitBuffer[11] = deviceGetStatus(DEVICE_ONOFF_2);
	frameTransmitBuffer[12] = '#';
	uartWrite(frameTransmitBuffer, 13);
	frameTransmitBuffer[0] = '*';
	frameTransmitBuffer[1] = FUNCTION_STATUS;
	frameTransmitBuffer[2] = DEVICE_ONOFF_3;
	frameTransmitBuffer[3] = 0x01;
	frameTransmitBuffer[11] = deviceGetStatus(DEVICE_ONOFF_3);
	frameTransmitBuffer[12] = '#';
	uartWrite(frameTransmitBuffer, 13);
}

void cmdDoStatusOne()
{
    frameTransmitBuffer[0] = '*';
	frameTransmitBuffer[1] = FUNCTION_STATUS;
	frameTransmitBuffer[2] = frameReceiveBuffer[2];
	frameTransmitBuffer[3] = 0x01;
	frameTransmitBuffer[11] = deviceGetStatus(frameReceiveBuffer[2]);
	frameTransmitBuffer[12] = '#';
	uartWrite(frameTransmitBuffer, 13);
}

/* CMD FRAME CLEAR
 * Clear the cmd receive frame, ready for next cmd.
 */
void cmdFrameClear()
{
	uint8_t loop;
	for (loop = 0; loop < 12; loop++)
	{
		frameReceiveBuffer[loop] = 0;
	}
	for (loop = 0; loop < 13; loop++)
	{
		frameTransmitBuffer[loop] = 0;
	}
}
