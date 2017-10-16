#include <ESP8266WiFi.h>
#include <os_type.h>
#include "WiFarm.h"
#include "fifo.h"
#include "mqtt.h"
#include "state.h"
#include "uart.h"

void buttonInit (void)
{
  pinMode(PIN_CONFIG, INPUT_PULLUP);
}

int buttonCheck (void)
{
  uint8_t xButton_Read = 0;
  if (!digitalRead(PIN_CONFIG))
  {
    #ifdef DEBUG
      Serial.println("bat dau nhan nut");
    #endif
    
    //int times = millis();
    int count = 0;
    xButton_Read = 1;
    delay(500);
    delay(500);

    if (!digitalRead(PIN_CONFIG))
    {
      xButton_Read &= 1;
    }
    else
      xButton_Read &= 0;
    delay(500);
    delay(500);
    if (!digitalRead(PIN_CONFIG))
    {
      xButton_Read &= 1;
    }
    else
    {
      xButton_Read &= 0;
      return 0;
    }
    delay(500);
    delay(500);
    if (!digitalRead(PIN_CONFIG))
    {
      xButton_Read &= 1;
    }
    else
    {
      xButton_Read &= 0;
      return 0;
    }
    if (xButton_Read == 1)
    {
      return 1;
    }
    else return 0;
  }
}


