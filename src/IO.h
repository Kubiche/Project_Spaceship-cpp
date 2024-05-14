#ifndef IO_H_
#define IO_H_

#include <Arduino.h>
#include <Joystick.h>
#include <MCP23X17.h>
#include <MCP300X.h>
#include <MAX72XX.h>
#include <SPI.h>
#include <Wire.h>



#define ANALOG_CHECK_INTERVAL 5 // Analog read interval to prevent USB saturation.

#define IO1_I2C_ADDRESS 0x21
#define IO2_I2C_ADDRESS 0x22
#define IO1_INT_PIN 5
#define IO2_INT_PIN 4

#define ADC_CS_PIN 9

#define BOOT_MODE_PIN 8

// The SPI Slave Select pin for the led controller
#define LED_DEV_COUNT 2
#define LED_CS 10


extern unsigned long analog_last_read;

void updateAnalogs();

void updateDigitals();

void initIO();

void getIO();

void pushIO();

void getSerialCommand();

void decodeCommand(uint8_t a, uint8_t b, uint8_t c);

#endif