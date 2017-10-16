#ifndef __ROOM_STACK_H
#define __ROOM_STACK_H
/*--- Define Name ---*/

/* for debuging */
#define DEBUG 0

/* define state */
#define STATE_GET_DATA_EEPROM 0
#define STATE_CONFIG          1
#define STATE_CONNECT_WIFI    2
#define STATE_CONTROLL        3
#define STATE_CONNECT_LOCAL   4
#define STATE_CONNECT_SERVER  5

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
#define ERROR_JSON
#define ERROR_TIMEOUT

#define FUNC_CONTROL     String("2")
#define FUNC_DATA        String("1")

typedef struct {
  uint32_t timeout;
  uint8_t recv_flag;
  uint8_t count;
  uint32_t timeCount;
  uint8_t send_flag;
  uint8_t send_timeout_flag;
  uint8_t send_timeCount;
} serial_recv_t;

/*--- Global Variable ---*/
//const char *ssid_config = "room_stack_device";
const char *password_config = "12345678";
char  ssid_config[] = "";
String user = "ESP8266";
uint8_t LED_flag = 0;
os_timer_t      Timer;        // kieu du lieu timer
fifo_t          FIFO;
serial_recv_t   UART;

/* buffer use for FIFO */
uint8_t gBuffer[FIFO_LENGTH][FIFO_WIDTH];
//uint8_t gBufferTx[FIFO_WIDTH];
//uint8_t gBufferRx[FIFO_WIDTH];

char ssid_connect[40] = "";
char password_connect[40] =  "";

String macID = "";

/* variable control state */
int g_state;
int g_next_state;

int flag_connect_wifi = 0;
int flag_config_wifi = 0;

/*--- Function ---*/

/*
 * get MACID of ESP8266
 */
String get_mac_id();

/* Init Serial, GPIO, Timer, FIFO */
void SystemInit();

/*
 * check if config button is pressed or not
 */
void button_config(); // bat nut config

/*
 * callback function, called when MQTT data available
 */
void callback(const MQTT::Publish& pub);

/*
 * useless state!
 */
void func_state_get_data_eeprom();

/*
 * state get ssid and password WIFI, save them in EEPROM
 */
void func_state_config();

/*
 * read ssid and pass from EEPROM, then connect WIFI
 */
void func_state_connect_wifi();


void func_state_controll();
void func_local();
void func_server();
void button_click();
void Blink();
int pasr_json(String json, uint8_t pOut[FIFO_WIDTH]);
String create_json(String sn, String function,  String controller, String address, String data);
void control_device (String data, String address, String function);
void getStatus_Device();
#endif
