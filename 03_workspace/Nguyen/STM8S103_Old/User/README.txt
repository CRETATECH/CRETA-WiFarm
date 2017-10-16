<==! FLEA - TEAM ==>
	* Project	:	F-001
	* Purpose	:	Application for Farming
	* Member	:	FLEA-TEAM
	* Date		:	18/3/2017

├── application-state		/*APP*/	register for process
│   ├── defineApplication.h
│   ├── stateApplication.c
│   └── stateApplication.h	
├── base-machine		/*Manager hardware*/
│   ├── defineHardware.h
│   ├── gpio.c
│   ├── gpio.h
│   ├── i2c.c
│   ├── i2c.h
│   ├── lcd.c
│   ├── lcd.h
│   ├── timer.c
│   ├── timer.h
│   ├── uart.c
│   └── uart.h
├── machine-running		/*SYSTEM-LOOP*/ -> register for APP
│   ├── defineMachine.h
│   ├── stateMachine.c
│   └── stateMachine.h
└── README.txt



