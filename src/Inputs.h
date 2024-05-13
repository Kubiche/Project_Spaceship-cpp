#pragma once

#include <Joystick.h>
#include <MCP23X17.h>
#include <MCP300X.h>


#define ANALOG_CHECK_INTERVAL 5 // Analog read interval to prevent USB saturation.

#define IO1_I2C_ADDRESS 0x21
#define IO2_I2C_ADDRESS 0x22
#define IO1_INT_PIN 5
#define IO2_INT_PIN 4

#define BOOT_MODE_PIN 8

extern unsigned long analog_last_read;

extern Joystick_ Joystick;

extern MCP23017 io1;

void updateAnalogs();

void updateDigitals();