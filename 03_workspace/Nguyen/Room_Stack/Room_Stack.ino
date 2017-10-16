/*--- Library Include ---*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <os_type.h>

#include <ArduinoJson.h>

#include "room_stack.h"

/*--- Declare Object ---*/
//const char* sserver = "pelab.ddns.net";
const char* sserver = "iot.eclipse.org";

WiFiServer    server(80);
WiFiClient    wclient;
PubSubClient  client(wclient, sserver);
void Blink()
{
  if (flag_connect_wifi == 1)
  {
    int temp = digitalRead(PIN_CONTROLL_DEVICE_1);
    digitalWrite(PIN_CONTROLL_DEVICE_1, !temp);
    digitalWrite(PIN_CONTROLL_DEVICE_2, !temp);
   // digitalWrite(PIN_CONTROLL_DEVICE_3, !temp);
  }

  if (flag_config_wifi == 1)
  {
    digitalWrite(PIN_CONTROLL_DEVICE_1, LOW);
    digitalWrite(PIN_CONTROLL_DEVICE_2, LOW);
    //digitalWrite(PIN_CONTROLL_DEVICE_3, LOW);
  }
//    Serial.println("vao");
//    if (WiFi.status() == WL_CONNECTED)
//    {
//    if (!client.connected())
//    {
//      if (client.connect(macID))
//      {
//        client.set_callback(callback);
//        client.subscribe(topic_in);
//        Serial.println("set callback");
//      }
//    }
//    }
//    if (client.connected())
//    client.loop();
}
/*--- MAIN MODULE ---*/
void setup()
{
  Serial.begin(115200);

  pinMode(PIN_CONFIG, INPUT_PULLUP);
  pinMode(PIN_CONTROLL_DEVICE_1, OUTPUT);
  pinMode(PIN_CONTROLL_DEVICE_2, OUTPUT);
  pinMode(PIN_CONTROLL_DEVICE_3, OUTPUT);
  macID = get_mac_id();
  String wifiname = "ESP" + macID;  
  //Serial.println(wifiname);
  wifiname.toCharArray(ssid_config, 14);
  //Serial.println(ssid_config);
  os_timer_disarm(&Timer);
  os_timer_setfn(&Timer, (os_timer_func_t *)Blink, NULL);
  os_timer_arm(&Timer, 1000, 1); // enable timer 1s overflow*/
  g_state = STATE_CONNECT_WIFI;

  delay(500);
  digitalWrite(PIN_CONTROLL_DEVICE_3, LOW);
  delay(10);
  digitalWrite(PIN_CONTROLL_DEVICE_3, HIGH);
}

void loop()
{
  switch (g_state)
  {
    case STATE_GET_DATA_EEPROM: func_state_get_data_eeprom(); break;
    case STATE_CONFIG: func_state_config();                   break;
    case STATE_CONNECT_WIFI: func_state_connect_wifi();       break;
    case STATE_CONTROLL: func_state_controll();               break;
  }
  g_state = g_next_state;
}

/*--- Function ---*/
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
  
  if (!digitalRead(PIN_CONFIG))
  {
    digitalWrite(PIN_CONTROLL_DEVICE_1, HIGH);
    digitalWrite(PIN_CONTROLL_DEVICE_2, HIGH);
   // digitalWrite(PIN_CONTROLL_DEVICE_3, HIGH);
    delay(200);
    int times = millis();
    int count = 0;
    while (!digitalRead(PIN_CONFIG))
      if (millis() - times > 1000)
      {
        count++;
        break;
      }
    while (!digitalRead(PIN_CONFIG))
      if (millis() - times > 1000)
      {
        count++;
        break;
      }
    while (!digitalRead(PIN_CONFIG))
      if (millis() - times > 1000)
      {
        count++;
        break;
      }
    if (count == 3)
    {
      g_next_state = STATE_CONFIG;
      //Serial.println("Tien hanh config");
    }
    //Serial.println("nhan nut");
  }
}

void callback(const MQTT::Publish& pub)
{
  String topic = pub.topic();
  String json = "";
  String data = "";
  String address = "";
  String function = "";
  String Status = "";
  String Year = "";
  String Month = "";
  String Day = "";
  String Hours = "";
  String Min = "";
    //Serial.println(topic);
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
//  Serial.print("du lieu nhan duoc___:");
//  Serial.print(json);
//  Serial.println("___");
  if (!pasr_json(json, data, address, function));
  //Serial.println("ko thanh cong");
  else
  {
    //control_device (data, address, function);
    send_data_mcu(data, address, function, Status, Year, Day, Month, Hours, Min);
  client.publish(MQTT::Publish("device/esp000003", create_json(idDevice, "2", data, address, function, user)).set_qos(0));
  }
}

void func_state_get_data_eeprom()
{
  button_config();
}

void func_state_config()
{
  //  Serial.println("////////////     State Config      ////////////");
  flag_config_wifi = 1;
  String req = "";
  String list = "";
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.softAP(ssid_config, password_config);
  

  server.begin();
  delay(2000);
  Serial.println("HTTP server started");
//  Serial.printf(ssid_config);
  do
  {
    wclient = server.available();
    delay(5);
  } while (!wclient);
  while (!wclient.available())
  {
    delay(1);
  }
  while (wclient.available())
  {
    req = wclient.readStringUntil('\r');
  }
  wclient.flush();
  req.remove(0, 2);
  //Serial.print("___du lieu nhan :  ");
  //Serial.println(req);
  int n = WiFi.scanNetworks();
  if (n == 0)// chua test truong hop nay
    Serial.println("no networks found");
  else
  {
    //Serial.print(n);
    //Serial.println(" networks found");
    list = "{\"list\":[";
    for (int i = 0; i < n; ++i)
    {
      list += "{\"number\":";
      list += i;
      list += ",\"name\":\"" + WiFi.SSID(i) + "\"}";
      if (i == n - 1)
      {
        list += "]}";
      }
      else
        list += ",";
    }
    //Serial.println(list);
  }
  String s = "HTTP/1.1 200 OK\r\n\r\n";
  s += list;
  wclient.print(s);
  wclient.stop();

  do
  {
    wclient = server.available();
    delay(5);
  } while (!wclient);
  while (!wclient.available())
  {
    delay(1);
  }
  while (wclient.available())
  {
    req = wclient.readStringUntil('\r');
  }
  wclient.flush();
  req.remove(0, 2);
  //Serial.println(req);
  int index = 0;
  for (int i = 0; i < req.length(); i++)
  {
    if (req.charAt(i) == '%')
      index = i;
  }
  //Serial.print("vi tri: ");
  //Serial.println(index);
  String ssid = WiFi.SSID((req.substring(0, index)).toInt());
  String pass = req.substring(index + 3, req.length());
  //Serial.println("Get SSID, PASS");
  ssid.toCharArray(ssid_connect, ssid.length() + 1);
  pass.toCharArray(password_connect, pass.length() + 1);
  //Serial.println(ssid_connect);
  //Serial.println(password_connect);

  s = "HTTP/1.1 200 OK\r\n\r\n";
  s += macID;
  //Serial.println(s);
  wclient.print(s);
  wclient.stop();



  /*------  Clear EEPROM ------*/
  EEPROM.begin(2000);
  for (int i = 0; i < 2000; i++)
    EEPROM.write(i, 0);
  index = ADDRESS_SSID;
  for (int i = 0; i < ssid.length(); i++)
  {
    EEPROM.write(index, ssid.charAt(i));
    index++;
  }

  index = ADDRESS_PASS;
  for (int i = 0; i < pass.length(); i++)
  {
    EEPROM.write(index, pass.charAt(i));
    index++;
  }

  EEPROM.end();
  g_next_state = STATE_CONNECT_WIFI;
  flag_config_wifi = 0;
  digitalWrite(PIN_CONTROLL_DEVICE_1, LOW);
  digitalWrite(PIN_CONTROLL_DEVICE_2, LOW);
  //digitalWrite(PIN_CONTROLL_DEVICE_3, LOW);
}

void func_state_connect_wifi()
{
  //Serial.println("////////////     State Connect wifi ////////////");
  g_next_state = STATE_CONTROLL;
  EEPROM.begin(2000);
  int index = ADDRESS_SSID;
  char temp;
  String strSSID = "";
  while (( temp = EEPROM.read(index)) != '\0')
  {
    strSSID += temp;
    index++;
    button_config();
    if (g_next_state == STATE_CONFIG)
      break;
  }
  strSSID.toCharArray(ssid_connect , strSSID.length() + 1 );

  String strPASS = "";
  index = ADDRESS_PASS;
  while (( temp = EEPROM.read(index)) != '\0')
  {
    strPASS += temp;
    index++;
    button_config();
    if (g_next_state == STATE_CONFIG)
      break;
  }
  strPASS.toCharArray(password_connect , strPASS.length() + 1 );


  topic_in  = "server/esp000003";
  topic_out = "device/esp000003";

  //Serial.println(ssid_connect);
  //Serial.println(password_connect);

  //Serial.println(topic_in);
  //Serial.println(topic_out);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_connect, password_connect);
//  WiFi.begin("p0828", "19930504");
//  WiFi.begin("Wifi3", "12345678");
  while (WiFi.status() != WL_CONNECTED)
  {
    flag_connect_wifi = 1 ;
    delay(500);
    button_config();
    if (g_next_state == STATE_CONFIG)
      break;
  }
 // Serial.println("da ket noi");
  flag_connect_wifi = 0;
  getStatus_Device();
  button_config();
}

void func_state_controll()
{
  //Serial.println("////////////     State Controll     ////////////");
  g_next_state = STATE_CONTROLL;
  button_config();
  if (WiFi.status() == WL_CONNECTED)
  {
//    if (!client.connected())
//    {
//      if (client.connect(macID))
//      {
//        client.set_callback(callback);
//        client.subscribe(topic_in);
//      }
//   }
//
//    if (client.connected())
//      client.loop();
    send_data_server();
//    delay(200);
  }
  else
  {
    g_next_state = STATE_CONNECT_WIFI;
  }
}
int pasr_json(String json, String& data, String& address , String& function)
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success())
  {
    return 0;
  }

  String data_buf        = root["data"];
  String address_buf     = root["address"];
  String function_buf    = root["function"];
  data       = data_buf;
  address    = address_buf;
  function   = function_buf;

  return 1;
}
void send_data_mcu(String data, String address, String function, String Status, String Year, String Day, String Month, String Hours, String Min)
{
  uint8_t data_send[10];

  data_send[0] = function.toInt(); // chua function
  data_send[1] = address.toInt(); // luu gia tri dia chi cua thiet bi con
  data_send[2] = Status.toInt(); // trang thai cua esp gui den mcu.
  data_send[3] = Year.toInt();
  data_send[4] = Month.toInt();
  data_send[5] = Day.toInt();
  data_send[6] = Hours.toInt();
  data_send[7] = Min.toInt();
  data_send[8] = (data.toInt() >> 8);
  data_send[9] = data.toInt() & 0xff;

  Serial.write('*');
  for (int i = 0; i < 10; i++)
    Serial.write(data_send[i]);
  //Serial.println(data_send[i]);
  Serial.write('#');
}
void send_data_server()
{
  uint8_t data_read[11];
  uint8_t i = 0;
  uint8_t val = 0;
  if (Serial.available())
  {
    if (Serial.read() == '*')
    {
      delay(5);
      while ( (val = Serial.read()) != '#')
      {
        data_read[i] = val;
        i++;
        delay(5);
      }

    }


    String data1       = (String)((data_read[9]) << 8 | data_read[10]);
    String address1    = (String)(data_read[1]);
    String function1   = (String)(data_read[0]);
    String Status1     = (String)(data_read[2]);
//    client.publish(MQTT::Publish("device/esp000006", create_json(idDevice, Status1, data1, address1, function1, user)).set_qos(0));

    if (function1.equals("11"))
    {
      
  
      if (address1.equals("1"))
      {
        if (data1.equals("100"))
          digitalWrite(PIN_CONTROLL_DEVICE_1, LOW);
        else
          digitalWrite(PIN_CONTROLL_DEVICE_1, HIGH);          
      }

      if (address1.equals("2"))
      {
        if (data1.equals("100"))
          digitalWrite(PIN_CONTROLL_DEVICE_2, LOW);
        else
          digitalWrite(PIN_CONTROLL_DEVICE_2, HIGH);          
      }

    }
    client.publish(MQTT::Publish("device/esp000003", create_json(idDevice, Status1, data1, address1, function1, user)).set_qos(0));

  }
}
String create_json(String id, String Status, String data, String address, String function, String user)
{
  String json = "{\"id\":\"" + id + "\",\"status\":\"" + Status + "\",\"data\":\"" + data + "\",\"address\":\"" + address + "\",\"user\":\"" + user + "\",\"function\":\"" + function + "\"}";
  return json;
}

void getStatus_Device()
{
  uint8_t data_send[10];
  data_send[0] = 11; // chua function
  data_send[1] = 0; // luu gia tri dia chi cua thiet bi con
  data_send[2] = 1; // trang thai cua esp gui den mcu.
  data_send[3] = 0;
  data_send[4] = 0;
  data_send[5] = 0;
  data_send[6] = 0;
  data_send[7] = 0;
  data_send[8] = 0;
  data_send[9] = 0;

  Serial.write('*');
  for (int i = 0; i < 10; i++)
    Serial.write(data_send[i]);
  //Serial.println(data_send[i]);
  Serial.write('#');
}







