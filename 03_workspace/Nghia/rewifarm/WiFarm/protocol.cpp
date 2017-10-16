#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <os_type.h>
#include "WiFarm.h"
#include "fifo.h"
#include "mqtt.h"
#include "state.h"
#include "uart.h"
#include "button.h"
#include "led.h"
#include "protocol.h"

extern fifo_t          FIFO;
extern uint8_t         gBuffer[FIFO_LENGTH][FIFO_WIDTH];

int protocolParseJson(String pJson, uint8_t* func, uint8_t* data, uint8_t *addr)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(pJson);
  if (!root.success())
  {
    return 1;
  }
  else
  {
    String vFunc       = root ["func"];
    String vAddr       = root ["addr"];
    String vData       = root ["data"];

    *data = vData.toInt();
    *func = vFunc.toInt();
    *addr = vAddr.toInt();
    return 0;
  }
}

void protocolJsonProc (String pJson)
{
  uint8_t vBuff[FIFO_WIDTH];
  uint8_t vFunc, vData, vAddr;
  uint8_t temp;
  temp = protocolParseJson (pJson, &vFunc, &vData, &vAddr);
  if (temp == 1) // Error parse json
  {
    protocolErrorRespond(ERR_PARSE_FAILED);
  }
  else // put FIFO
  {
    preBuffer(vFunc, vAddr, vData, vBuff);
    if (xFIFO_Send(gBuffer, vBuff, &FIFO) == 0)
    {
      #ifdef DEBUG
        Serial.println("fifo send ok");
      #endif 
    }
    else
      protocolErrorRespond(ERR_FIFO_FULL);
  }
}

void protocolErrorRespond(uint8_t Error)
{
  mqttPublish(254, 0, Error);
}

void preBuffer (uint8_t func, uint8_t addr, uint8_t data, uint8_t *pBuff)
{
  pBuff[START_BYTE] = 0x27;
  pBuff[FUNC]       = func;
  pBuff[ADDR]       = addr;
  pBuff[YEAR]       = 0;
  pBuff[MON]        = 0;
  pBuff[DAY]        = 0;
  pBuff[HOUR]       = 0;
  pBuff[MIN]        = 0;
  pBuff[DATA_H]     = 0;
  pBuff[DATA_L]     = data;
}

 
