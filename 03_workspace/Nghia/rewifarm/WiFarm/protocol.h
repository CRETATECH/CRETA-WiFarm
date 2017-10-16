#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define START_BYTE     0
#define FUNC           1
#define ADDR           2
#define YEAR           3
#define MON            4
#define DAY            5
#define HOUR           6
#define MIN            7
#define DATA_H         8
#define DATA_L         9
#define END_BYTE       10

void protocolJsonProc (String pJson);
int protocolParseJson(String pJson, uint8_t* func, uint8_t* data, uint8_t *addr);
void protocolErrorRespond(uint8_t Error);
void preBuffer (uint8_t func, uint8_t addr, uint8_t data, uint8_t *pBuff);

typedef enum {
  ERR_PARSE_FAILED = 1,
  ERR_STM_TIMEOUT = 2,
  ERR_FIFO_FULL = 3,
  
  ERR_KEY = 5,
  ERR_SENSOR,
}error_t;

#endif
