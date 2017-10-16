#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "WiFarm.h"
#include "fifo.h"
#include "uart.h"
#include "mqtt.h"
#include "protocol.h"

const char* sserver = "iot.eclipse.org";
WiFiServer    server(80);
WiFiClient    wclient;
PubSubClient  client(wclient, sserver);



void callback(const MQTT::Publish& pub)
{
  String topic = pub.topic();
  String json = ""; //chua du lieu nhan duoc
  if (pub.has_stream())
  {
    uint8_t buf[BUFFER_SIZE];
    int read;
    while (read = pub.payload_stream()->read(buf, BUFFER_SIZE))
    {
      //Serial.write(buf, read);
    }
    pub.payload_stream()->stop();
  }
  else
    json = pub.payload_string();
    
  #ifdef DEBUG  
    Serial.print("du lieu nhan duoc___:");
    Serial.print(json);
    Serial.println("___");
  #endif
  protocolJsonProc(json);
}

int mqttCheckConnect(void)
{
  return client.connected();
}

void mqttLoop(void)
{
  client.loop();
}

void mqttSubscribe(void)
{
  client.subscribe(TOPIC_IN);
}

void mqttPublish (uint8_t pFunc, uint8_t pAddr, uint8_t pData)
{
  String func = (String)pFunc;
  String addr = (String)pAddr;
  String data = (String)pData;
  client.publish(MQTT::Publish(TOPIC_OUT, create_json(data, addr, func)).set_qos(0));
}

int mqttConnect (void)
{
  return client.connect("nghia");
}

void mqttSetCallback(void)
{
  client.set_callback(callback);
}

String create_json(String data, String address, String function)
{
  String json = "{\"user\":\"ESP8266\",\"func\":\"" + function + "\",\"addr\":\"" + address + "\",\"year\":\"0\",\"mon\":\"0\",\"day\":\"0\",\"hour\":\"0\",\"min\":\"0\",\"data\":\"" + data + "\"}";
  return json;
}

