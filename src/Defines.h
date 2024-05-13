#ifndef DEFINES_H_
#define DEFINES_H_

//#define DEBUG 

#ifdef DEBUG
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debuglnB(x) Serial.println(x, BIN)
#else
#define debug(x)
#define debugln(x)
#define debuglnB(x) 
#endif   

// The SPI Slave Select pin for the led controller
#define LED_CS 10


//device number in order from last to first on cascade

#define LED_bars_board 1 
#define LED_single_board 0 


#define ANALOG_CHECK_INTERVAL 5 // Analog read interval to prevent USB saturation.

#define IO1_I2C_ADDRESS 0x21
#define IO2_I2C_ADDRESS 0x22
#define IO1_INT_PIN 5
#define IO2_INT_PIN 4

#define BOOT_MODE_PIN 8

#endif