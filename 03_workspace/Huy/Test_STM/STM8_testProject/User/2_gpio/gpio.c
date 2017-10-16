
#include "gpio.h"

/*************** System Clock ****************/

    
uint32_t clock;
uint32_t count;

/**
  * Function Name: gpioInit
  * Description:   init gpio
  * Input:         none
  * Output:        none
  * Return:        void
  */
void gpioInit()
{
  GPIO_Init( (LED_GPIO_PORT), LED_GPIO_PIN , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init( (BUZZ_GPIO_PORT), BUZZ_GPIO_PIN , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init( (RELAY_GPIO_PORT), RELAY_GPIO_PIN_1|RELAY_GPIO_PIN_2 , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init( (SW_GPIO_PORT ), SW_GPIO_PIN_1|SW_GPIO_PIN_2 , GPIO_MODE_IN_PU_NO_IT);
}
/**
  * Function Name: blinky
  * Description:   blinky led
  * Input:         none
  * Output:        none
  * Return:        void
  */
void Delay()
{
  uint16_t nCount = 0xFFFF;
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
void blinky()
{
    /* Toggles LEDs */
    GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN);
    Delay();
}
/**
  * Function Name: ledOn
  * Description:   turn on Led 
  * Input:         none
  * Output:        none
  * Return:        void
  */

void ledOn()
{
    GPIO_WriteLow(LED_GPIO_PORT, LED_GPIO_PIN);
}
/**
  * Function Name: ledOff
  * Description:   turn Off led
  * Input:         none
  * Output:        none
  * Return:        void
  */

void ledOff()
{
    GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PIN);
}
/**
  * Function Name: buzzer
  * Description:   turn on buzzer 1s then turn off buzzer delay 1s then exit
  * Input:         none
  * Output:        none
  * Return:        void
  */

void buzzer()
{
    GPIO_WriteHigh(BUZZ_GPIO_PORT, BUZZ_GPIO_PIN);
    Delay();
    GPIO_WriteLow(BUZZ_GPIO_PORT, BUZZ_GPIO_PIN);
}
/**
  * Function Name: relay_1
  * Description:   turn on relay_1 1s then turn off relay_1 delay 1s then exit
  * Input:         none
  * Output:        none
  * Return:        void
  */
void relay_1()
{
    GPIO_WriteHigh(RELAY_GPIO_PORT, RELAY_GPIO_PIN_1);
    Delay();
    GPIO_WriteLow(RELAY_GPIO_PORT, RELAY_GPIO_PIN_1);
}
/**
  * Function Name: relay_2
  * Description:   turn on relay_2 1s then turn off relay_2 delay 1s then exit
  * Input:         none
  * Output:        none
  * Return:        void
  */
void relay_2()
{
    GPIO_WriteHigh(RELAY_GPIO_PORT, RELAY_GPIO_PIN_2);
    Delay();
    GPIO_WriteLow(RELAY_GPIO_PORT, RELAY_GPIO_PIN_2);
}
/**
  * Function Name:  button_1Check
  * Description:   check status button1
  * Input:         none
  * Output:        none
  * Return:        1 (if button1 not pressed) // 0 (if button1 pressed)
  */
int button_1Check()
{
    if (GPIO_ReadInputPin(SW_GPIO_PORT, SW_GPIO_PIN_1))
      return 1;
    else
      return 0;
}
/**
  * Function Name:  button_2Check
  * Description:   check status button2
  * Input:         none
  * Output:        none
  * Return:        1 (if button2 not pressed) // 0 (if button2 pressed)
  */
int button_2Check()
{
    if (GPIO_ReadInputPin(SW_GPIO_PORT, SW_GPIO_PIN_2))
      return 1;
    else
      return 0;
}
/**
  * Function Name:  sw_1Check
  * Description:   if sw1 pressed blick led (1s) // if not exit after 5s
  * Input:         none
  * Output:        none
  * Return:        0
  */
int sw_1Check()
{
  uint16_t count = 20;
  while(button_1Check())
  {
    count--;
    if(count == 0)
      return 0;
    Delay();
  }
  if (!button_1Check())
  {
    ledOn();
    Delay();
    ledOff();
  }
  return 0;
}  
/**
  * Function Name:  sw_2Check
  * Description:   if sw2 pressed blick led (1s) // if not exit after 5s
  * Input:         none
  * Output:        none
  * Return:        0
  */
int sw_2Check() // button 2
{
    uint16_t count = 20;
    while(button_2Check())
    {
      count--;
      if(count == 0)
        return 0;
      Delay();
    }
    if (!button_2Check())
    {
      ledOn();
      Delay();
      ledOff();
    }
    return 0;
}




