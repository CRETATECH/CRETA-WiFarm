#ifndef __ROOM_STACK_H
#define __ROOM_STACK_H
/*--- Define Name ---*/
// Define State
#define STATE_GET_DATA_EEPROM 0
#define STATE_CONFIG          1
#define STATE_CONNECT_WIFI    2
#define STATE_CONTROLL        3

// Define Pin
#define PIN_CONFIG 0
#define PIN_CONTROLL_DEVICE_1 5
#define PIN_CONTROLL_DEVICE_2 4
#define PIN_CONTROLL_DEVICE_3 13

// Define buffer
#define ADDRESS_SSID 100
#define ADDRESS_PASS 200
#define ADDRESS_ROOM 300
#define ADDRESS_DEVICE 400

#define BUFFER_SIZE 1000
#define GEODE       12



/*--- Global Variable ---*/
//const char *ssid_config = "room_stack_device";
const char *password_config = "12345678";
char  ssid_config[] = "";
os_timer_t      Timer;        // kieu du lieu timer

String idDevice = "esp003";
String user = "khai_dep_trai";

char ssid_connect[40] = "";
char password_connect[40] =  "";

String topic_in  = "";
String topic_out = "";

int g_state;
int g_next_state;

int flag_connect_wifi = 0;

int flag_config_wifi = 0;
String macID = "";
/*--- Function ---*/
String get_mac_id();
void button_config(); // bat nut config
void callback(const MQTT::Publish& pub);
void func_state_get_data_eeprom();
void func_state_config();
void func_state_connect_wifi();
void func_state_controll();

void button_click();

int pasr_json(String json, uint8_t& address, String& data, String function );
String create_json(String id, String Status, String data, String device, String function);
void control_device (String data, String address, String function);
void getStatus_Device();
#endif
