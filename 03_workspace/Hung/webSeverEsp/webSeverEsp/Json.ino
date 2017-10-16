
#include"Json.h"
#include <ArduinoJson.h>

void pasr_json(String JSONMessage, String& data, String& address , String& function)
{
  StaticJsonBuffer<300> JSONBuffer;   //Memory pool
  JsonObject& parsed = JSONBuffer.parseObject(JSONMessage); //Parse message
 
  if (!parsed.success()) {   //Check for errors in parsing
 
    Serial.println("Parsing failed");
    delay(5000);
    return;
 
  }
 
  String data_buf        = parsed["data"];
  String address_buf     = parsed["address"];
  String function_buf    = parsed["function"];
  data       = data_buf;
  address    = address_buf;
  function   = function_buf;                                        
}


