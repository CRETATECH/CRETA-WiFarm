HOW TO USE TEST PROJECT

1/ Connect ST-Link and STM8 (RST, SWIM, VCC, GND)
2/ Open "test.eww" in STM8_test\Project.
3/ In group "1_main\main.c", open "gpio.h" (right click on "gpio.h" then chosse open), edit GPIO_TEST_PORT and GPIO_TEST_PIN
to select which pin connect to led. Save.

#define LED_GPIO_PORT  GPIOB         // khai bao lai port muon test (GPIOA)
#define LED_GPIO_PIN   GPIO_PIN_5    // khai bao lai pin muon test  (GPIO_PIN_1)

4/ Make (F7).
5/ Download (Ctrl+D).
6/ Run (F5).
7/ If everything ok the led will blink.

Contact me if you have any problem or need test code for other modules.
Hung_0964408608_0904021706.