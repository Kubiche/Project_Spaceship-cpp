#ifndef IO_H_
#define IO_H_

#include <Arduino.h>
#include <Joystick.h>
#include <MCP23X17.h>
#include <MCP300X.h>
#include <MAX72XX.h>
#include <SPI.h>
#include <Wire.h>


// Analog read interval to prevent USB saturation.
#define ANALOG_CHECK_INTERVAL 5
//Joystick update interval
#define JOYSTICK_UPDATE_INTERVAL 10 
// I2C address of the first IO IC
#define IO1_I2C_ADDRESS 0x21 
// I2C address of the second IO IC
#define IO2_I2C_ADDRESS 0x22
// Interrup Pin for the first IO IC
#define IO1_INT_PIN 5
// Interrup Pin for the second IO IC
#define IO2_INT_PIN 4
// The Chip Select pin for the ADC IC
#define ADC_CS_PIN 9
// Pin for an optional boot mode
#define BOOT_MODE_PIN 8
// The number of devices cascaded including the first
#define LED_DEV_COUNT 2
// The SPI Slave Select pin for the led controller
#define LED_CS 10


void updateAnalogs();
void updateDigitals();
void initIO();
void getIO();
void pushIO();
void getSerialCommand();
void decodeCommand(unsigned char (&buffer)[3]);

#endif