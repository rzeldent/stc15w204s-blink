#pragma once


#ifdef DEMO_BOARD_STC15W204S
    // STC15W204S
    #define LED_BUILDIN P33
    #define LED_BUILDIN_ON  1
    #define LED_BUILDIN_OFF  0

    #define SWITCH_GND P32

    #define I2C_SCL P55
    #define I2C_SDA P54
#endif

#ifdef DEMO_BOARD_STC15W408AS
    // STC15W408AS
    #define LED_BUILDIN P10 
    #define LED_BUILDIN_ON  0
    #define LED_BUILDIN_OFF  1

    #define SWITCH_GND P32

    #define I2C_SCL P11
    #define I2C_SDA P12
#endif