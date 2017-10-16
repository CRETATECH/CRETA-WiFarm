/*--- Library Include ---*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <os_type.h>
#include <ArduinoJson.h>
#include "fifo.h"
#include "room_stack.h"

WiFiUDP Udp;
/*--- Declare Object ---*/
//const char* sserver = "pelab.ddns.net";
const char* sserver = "iot.eclipse.org";
WiFiServer    server(80);
WiFiClient    wclient;
PubSubClient  client(wclient, sserver);

/*--- MAIN MODULE ---*/
void setup()
{

  /* Init Serial, GPIO, Timer, FIFO */
  SystemInit();

  /* get macID of ESP and make it become wifiname */
  macID = get_mac_id();
  String wifiname = "ESP" + macID;  
  wifiname.toCharArray(ssid_config, 14);
  Serial.println(wifiname);

  /* first state of system */
  g_state = STATE_CONNECT_WIFI;
  //g_state = STATE_CONFIG;
  /* reset STM because it cant */
//  delay(500);
//  digitalWrite(PIN_CONTROLL_DEVICE_3, LOW);
//  delay(10);
//  digitalWrite(PIN_CONTROLL_DEVICE_3, HIGH);
}

void loop()
{
  switch (g_state)
  {
    case STATE_GET_DATA_EEPROM: func_state_get_data_eeprom(); break;
    case STATE_CONFIG: func_state_config();                   break;
    case STATE_CONNECT_WIFI: func_state_connect_wifi();       break;
    case STATE_CONTROLL: func_state_controll();               break;
    case STATE_CONNECT_LOCAL: func_local();                   break;
    case STATE_CONNECT_SERVER: func_server();                 break;
    default: break;
  }
  g_state = g_next_state;
}

/*--- Function ---*/

void callback(const MQTT::Publish& pub)
{
  uint8_t vBufferRx[FIFO_WIDTH];
  String topic = pub.topic();
  String json = ""; //chua du lieu nhan duoc
  if (pub.has_stream())
  {
    uint8_t buf[BUFFER_SIZE];
    int read;
    while (read = pub.payload_stream()->read(buf, BUFFER_SIZE))
    {
      Serial.write(buf, read);
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
  
  if (!pasr_json(json, vBufferRx))
  {
    #ifdef DEBUG
      Serial.println("pars failed");
    #endif
    client.publish(MQTT::Publish(topic_out, create_json("1", "0", "254", "ESP8266", "0", "0", "0", "0", "0")).set_qos(0));
  }
  else
  {
    if (xFIFO_Send(gBuffer, vBufferRx, &FIFO) == 0)
    {
      #ifdef DEBUG
      Serial.println("fifo send ok");
      #endif 
    }
    else
    {
		  /* them truong hop fifo full */
     client.publish(MQTT::Publish(topic_out, create_json("4", "0", "254", "ESP8266", "0", "0", "0", "0", "0")).set_qos(0));
      #ifdef DEBUG
      Serial.println("fifo full");
      #endif
    }
  }
}

/*-----------------------------------------*/
void func_state_config()
{
  LED_flag = 3;
  //WiFi.disconnect();
  #ifdef DEBUG
    Serial.println("State Config");
  #endif
  
  button_config();
  if (g_next_state == STATE_CONNECT_WIFI)
    return;
  int cnt = 0;
  WiFi.mode(WIFI_STA);
  //Chờ kết nối
//  while(WiFi.status() != WL_CONNECTED) {
//    delay(600);
//    //Serial.print(".");
//    if(cnt++ >= 10){
//       WiFi.beginSmartConfig();
//       while(1){
//           delay(1000);
//           button_config();
//           if (g_next_state == STATE_CONNECT_WIFI)
//              return;
//           //Kiểm tra kết nối thành công in thông báo
//           if(WiFi.smartConfigDone()){
//             //Serial.println("SmartConfig Success");
//             break;
//           }
//       }
//    }
//  }
  delay(300);
  WiFi.beginSmartConfig();
  while(1)
  {
    button_config();
    if (g_next_state == STATE_CONNECT_WIFI)
    {
      WiFi.stopSmartConfig();
      return;
    }
    delay(1000);
    if (WiFi.smartConfigDone())
      break;
  }
  



  //#ifdef DEBUG
    //Serial.println("");
    //Serial.println("");
  
    WiFi.printDiag(Serial);
  
    // Khởi tạo server
    //Udp.begin(49999);
    //Serial.println("Server started");

    // In địa chỉ IP
    //Serial.println(WiFi.localIP());
  //#endif
 
  g_next_state = STATE_CONTROLL;


}

void func_state_connect_wifi()
{
  LED_flag = 1;
  #ifdef DEBUG
    Serial.println("State Connect wifi");
  #endif
  topic_in  = "server/esp000021";
  topic_out = "device/esp000021";
  g_next_state = STATE_CONTROLL;

  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    #ifdef DEBUG
      Serial.print(".");
    #endif

    delay(100);
    button_config();
    if (g_next_state == STATE_CONFIG)
      return;
  }

  LED_flag = 2;
  if (client.connect(macID))
  {
      client.set_callback(callback);
      client.subscribe(topic_in);
  }
  client.publish(MQTT::Publish(topic_out, create_json("0", "0", "4", "ESP8266", "0", "0", "0", "0", "0")).set_qos(0));
}

/*
 * Function Control
 * control sending data to STM and server
 * control where to connect (wifi, server, local)
 * detail: if lost connect to access point (wifi) -> next state is CONNECT WIFI
 *         if lost connect to server MQTT         -> next state is LOCAL
 *         if connect to server                   -> next state is SERVER
 */
void func_state_controll()
{
  LED_flag = 2;
  button_config();
  if (g_next_state == STATE_CONFIG)
    return;
  /* process Buffer and send to STM if buffer is not empty */
  send_data_mcu();
  
  g_next_state = STATE_CONTROLL;
  

  
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected())
    {
      /* try to conncet to server */
      if (client.connect(macID))
      {
        client.set_callback(callback);
        client.subscribe(topic_in);
      }
      /* change state */
      g_next_state = STATE_CONNECT_LOCAL;
    }
    if (client.connected())
    {
      client.loop();
      g_next_state = STATE_CONNECT_SERVER;
    }
  }
  else
  {
    g_next_state = STATE_CONNECT_WIFI;
  }
}

void func_local()
{
  g_next_state = STATE_CONTROLL;
  /* do sth in local */
}

void func_server()
{
  g_next_state = STATE_CONTROLL;

  /* do sth in server */
  send_data_server();
}
int pasr_json(String json, uint8_t pOut[FIFO_WIDTH])
{
  //StaticJsonBuffer<1000> jsonBuffer;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success())
  {
    return 0;
  }

  String data        = root["data"];
  String address     = root["addr"];
  String function    = root["func"];
  String year        = root["year"];
  String mon         = root["mon"];
  String day         = root["day"];
  String hour        = root["hour"];
  String minute      = root["min"];


  pOut[0] = function.toInt();
  pOut[1] = address.toInt();
  pOut[2] = year.toInt();
  pOut[3] = mon.toInt();
  pOut[4] = day.toInt();
  pOut[5] = hour.toInt();
  pOut[6] = minute.toInt();;
  pOut[7] = data.toInt() >> 8;
  pOut[8] = data.toInt() & 0xFF;
  return 1;
}
void send_data_mcu()
{
  uint8_t vBufferTx[FIFO_WIDTH];
  if (UART.send_flag == 0)
  {
      if (xFIFO_Get (gBuffer, vBufferTx, &FIFO) == 0)
    {
      #ifdef DEBUG
        Serial.println("FIFO available, start transmitting");
      #endif
    
      Serial.write('*');
      for (int i = 0; i < FIFO_WIDTH; i++)
        Serial.write(vBufferTx[i]);
      Serial.write('#');
      UART.send_flag = 1;
    }
    else
    {
      /* do sth when fifo empty, normaly do nothing! */
    }
  }

}
void send_data_server()
{
  uint8_t data_read[11];
  uint8_t i = 0;
  uint8_t val = 0;
  String address = "";
  String function = "";
  String year = "";
  String mon = "";
  String day = "";
  String hour = "";
  String minute = "";
  String data = "";
  if (UART.send_timeout_flag == 1)
  {
    UART.send_timeout_flag = 0;
    UART.send_flag = 0;
    UART.send_timeCount = 0;
    #ifdef DEBUG
      Serial.println("time out error 1");
    #endif
    /* timeout, return error */
    //serialClear();
    address = "0";
    function = "254";
    year = "0";
    mon = "0";
    day = "0";
    hour = "0";
    minute = "0";
    data = "2";
    client.publish(MQTT::Publish(topic_out, create_json(data, address, function, user, year, mon, day, hour, minute)).set_qos(0));
    #ifdef DEBUG
      Serial.println("publish xong");
    #endif
    return;    
  }
  if (Serial.available() == 0)
    return;
  else
  {
    UART.send_flag = 0;
    UART.send_timeCount = 0;
    while (UART.count != 11)
    {
      if (Serial.available() != 0)
      {
        data_read[UART.count] = Serial.read();
        UART.count++;
      }
      UART.timeCount++;
      if (UART.timeCount > UART.timeout)
      {
        UART.recv_flag = 1;
        break;
      }
    }
    if (UART.recv_flag == 1)
    {
      #ifdef DEBUG
        Serial.println("time out error 2");
      #endif
      /* timeout, return error */
      serialClear();
      address = "0";
      function = "254";
      year = "0";
      mon = "0";
      day = "0";
      hour = "0";
      minute = "0";
      data = "5";
      client.publish(MQTT::Publish(topic_out, create_json(data, address, function, user, year, mon, day, hour, minute)).set_qos(0));
    }
    else
    {
      if ( (data_read[0] == '*') && (data_read[10] == '#') )
        {
          #ifdef DEBUG
            Serial.println("serial recv ok");
          #endif
          /* serial recv ok, send to server */
          address    = (String)(data_read[2]);
          function   = (String)(data_read[1]);
          year       = (String)(data_read[3]);
          mon        = (String)(data_read[4]);
          day        = (String)(data_read[5]);
          hour       = (String)(data_read[6]);
          minute     = (String)(data_read[7]);
          data       = (String)((data_read[8]) << 8 | data_read[9]);
          client.publish(MQTT::Publish(topic_out, create_json(data, address, function, user, year, mon, day, hour, minute)).set_qos(0));
       }
       else
       {
          #ifdef DEBUG
            Serial.println("serial recv not ok, '*' or '#' is wrong");
          #endif
          /* send error to server */
          address = "0";
          function = "254";
          year = "0";
          mon = "0";
          day = "0";
          hour = "0";
          minute = "0";
          data = "3";
          client.publish(MQTT::Publish(topic_out, create_json(data, address, function, user, year, mon, day, hour, minute)).set_qos(0));        
       }
    }
    UART.count = 0;
    UART.timeCount = 0;
    UART.recv_flag = 0;
  }
}    


String create_json(String data, String address, String function, String user, String year, String mon, String day, String hour, String minute)
{
  String json = "{\"user\":\"" + user + "\",\"func\":\"" + function + "\",\"addr\":\"" + address + "\",\"year\":\"" + year + "\",\"mon\":\"" + mon + "\",\"day\":\"" + day + "\",\"hour\":\"" + hour + "\",\"min\":\"" + minute + "\",\"data\":\"" + data + "\"}";
  return json;
}

void getStatus_Device()
{
  uint8_t data_send[10];
  data_send[0] = 6; // chua function
  data_send[1] = 0; // luu gia tri dia chi cua thiet bi con
  data_send[2] = 0; // trang thai cua esp gui den mcu.
  data_send[3] = 0;
  data_send[4] = 0;
  data_send[5] = 0;
  data_send[6] = 0;
  data_send[7] = 0;
  data_send[8] = 0;

  Serial.write('*');
  for (int i = 0; i < FIFO_WIDTH; i++)
    Serial.write(data_send[i]);
  Serial.write('#');
}

void SystemInit()
{
  Serial.begin(115200);

  pinMode(PIN_CONFIG, INPUT_PULLUP);
  pinMode(PIN_CONTROLL_DEVICE_1, OUTPUT);
  digitalWrite(PIN_CONTROLL_DEVICE_1, HIGH);
  //pinMode(PIN_CONTROLL_DEVICE_2, OUTPUT);
  //pinMode(PIN_CONTROLL_DEVICE_3, OUTPUT);

  os_timer_disarm(&Timer);
  os_timer_setfn(&Timer, (os_timer_func_t *)Blink, NULL);
  os_timer_arm(&Timer, 200, 1); // enable timer 1s overflow*/
  
  /* Init FIFO */
  xFIFO_Init(&FIFO);

  UART.timeout = 5000;
  UART.recv_flag = 0;
  UART.count = 0;
  UART.send_flag = 0;
  UART.send_timeCount = 0;
  UART.send_timeout_flag = 0;
}

String get_mac_id()
{
  String val;
  byte mac[6];
  WiFi.macAddress(mac);
  val = String(mac[0], HEX) + String(mac[1], HEX) + String(mac[2], HEX) + String(mac[3], HEX) + String(mac[4], HEX) + String(mac[5], HEX);
  return val;
}

void button_config()
{
  //LED_flag = 3;
  uint8_t xButton_Read = 0;
  if (!digitalRead(PIN_CONFIG))
  {
    #ifdef DEBUG
      Serial.println("bat dau nhan nut");
    #endif
    //digitalWrite(PIN_CONTROLL_DEVICE_1, HIGH);
    
    int times = millis();
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
      return;
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
      return;
    }
    if (xButton_Read == 1)
    {
      if ((g_state == STATE_CONTROLL)| (g_state == STATE_CONNECT_WIFI))
      {
        g_next_state = STATE_CONFIG;
        #ifdef DEBUG
          Serial.println("tien hanh config");
        #endif
      }
      else
      {
        g_next_state = STATE_CONNECT_WIFI;
        #ifdef DEBUG
          Serial.println("tien hanh ket noi wifi");
        #endif   
      }
    }
  }
}

void Blink()
{
  if (LED_flag == 1) // connecting wifi
  {
    int temp = digitalRead(PIN_CONTROLL_DEVICE_1);
    digitalWrite(PIN_CONTROLL_DEVICE_1, !temp);
  }
  if (LED_flag == 2) //connected wifi
  {
    digitalWrite(PIN_CONTROLL_DEVICE_1, LOW);
  }
  if (LED_flag == 3) // no wifi connected
  {
    digitalWrite(PIN_CONTROLL_DEVICE_1, HIGH);
  }
//  if (flag_connect_wifi == 1)
//  {
//    int temp = digitalRead(PIN_CONTROLL_DEVICE_1);
//    digitalWrite(PIN_CONTROLL_DEVICE_1, !temp);
//    //digitalWrite(PIN_CONTROLL_DEVICE_2, !temp);
//  }
//
//  if (flag_config_wifi == 1)
//  {
//    digitalWrite(PIN_CONTROLL_DEVICE_1, LOW);
//    //digitalWrite(PIN_CONTROLL_DEVICE_2, LOW);
//  }
  if (UART.send_flag == 1)
  {
    UART.send_timeCount++;
  }
  if (UART.send_timeCount > 2)
    UART.send_timeout_flag = 1;
}

void func_state_get_data_eeprom()
{
  button_config();
}

void serialClear (void)
{
  uint8_t temp;
  while (Serial.available() != 0)
  {
    temp = Serial.read();
  }
}

