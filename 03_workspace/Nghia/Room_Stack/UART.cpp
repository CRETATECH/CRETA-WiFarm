#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <os_type.h>
#include <ArduinoJson.h>
#include "fifo.h"
#include "room_stack.h"
#include "UART.h"


void 
