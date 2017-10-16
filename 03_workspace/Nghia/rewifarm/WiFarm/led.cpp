#include <ESP8266WiFi.h>
#include "WiFarm.h"

void ledInit (void)
{
  pinMode (PIN_LED, OUTPUT);
}

void ledOn (void)
{
  digitalWrite (PIN_LED, LOW);
}

void ledOff (void)
{
  digitalWrite (PIN_LED, HIGH);
}

void ledToggle (void)
{
  int temp = digitalRead (PIN_LED);
  digitalWrite(PIN_LED, !temp);
}

