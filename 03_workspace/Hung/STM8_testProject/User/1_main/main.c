#include "gpio.h"


void main()
{
  gpioInit();
  while(1)
  {
    blinky();
  }
}


