#include <ESP8266WiFi.h>
#include <os_type.h>
#include "WiFarm.h"
#include "fifo.h"
#include "mqtt.h"
#include "state.h"
#include "uart.h"
#include "button.h"
#include "led.h"

uint8_t         gBuffer[FIFO_LENGTH][FIFO_WIDTH];
fifo_t          FIFO;
uint8_t         gState;
os_timer_t      Timer; 
uint8_t         gLed;

void stateSetup(void)
{
  Serial.begin(115200);

  buttonInit();
  ledInit();

  os_timer_disarm(&Timer);
  os_timer_setfn(&Timer, (os_timer_func_t *)Blink, NULL);
  os_timer_arm(&Timer, 200, 1); // enable timer 1s overflow*/

  xFIFO_Init(&FIFO);
}

void stateControl (void)
{
  gLed = LED_ON;
  if (buttonCheck() == 1)
  {
    gState = STATE_CONFIG;
    return;
  }
    if (WiFi.status() == WL_CONNECTED)
    {
      if(!mqttCheckConnect())
      {
        if (mqttConnect())
        {
          mqttSetCallback();
          mqttSubscribe();
        }
      }
      else
      {
        mqttLoop();
      }
    }
    else
    {
      gState = STATE_CONNECT_WIFI;
    }
}

void stateConnectWifi(void)
{
  gLed = LED_BLINK;
  WiFi.begin();
  
  while (WiFi.status() != WL_CONNECTED)
  {
    if (buttonCheck() == 1)
    {
      gState = STATE_CONFIG;
      return;
    }
    #ifdef DEBUG
      Serial.print(".");
    #endif
    delay(100);
  }
  gState = STATE_CONTROL;
}

void stateConfig(void)
{
  gLed = LED_OFF;
  WiFi.mode(WIFI_STA);
  delay(300);
  WiFi.beginSmartConfig();
  while(1)
  {
    if (buttonCheck() == 1)
    {
      gState = STATE_CONNECT_WIFI;
      WiFi.stopSmartConfig();
      return;
    }
    delay(1000);
    if (WiFi.smartConfigDone())
      break;
  }
}

void Blink (void)
{
  if (gLed == LED_BLINK)
    ledToggle();
  else if (gLed == LED_ON)
    ledOn();
  else ledOff();
}

