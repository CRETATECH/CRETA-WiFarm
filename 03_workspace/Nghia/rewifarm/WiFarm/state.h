#ifndef STATE_H_
#define STATE_H_


#define STATE_CONFIG          1
#define STATE_CONNECT_WIFI    2
#define STATE_CONTROL         3
#define LED_BLINK             1
#define LED_ON                2
#define LED_OFF               3

void Blink (void);
void stateSetup(void);

#endif
