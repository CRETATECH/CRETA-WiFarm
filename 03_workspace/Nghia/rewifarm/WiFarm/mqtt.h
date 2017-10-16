#ifndef MQTT_H_
#define MQTT_H_



int mqttCheckConnect(void);
void mqttLoop(void);
void mqttSubscribe(void);
int mqttConnect (void);
void mqttSetCallback(void);
String create_json(String data, String address, String function);
void mqttPublish (uint8_t pFunc, uint8_t pAddr, uint8_t pData);
#endif
