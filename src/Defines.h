
#pragma once

// The SPI Slave Select pin for the led controller
#define LED_CS 10


//device number in order from last to first on cascade
#define Controller_v2 1
#define Controller_v3 0


#define ANALOG_CHECK_INTERVAL 5 // Analog read interval to prevent USB saturation.

#define IO1_I2C_ADDRESS 0x21
#define IO2_I2C_ADDRESS 0x22
#define IO1_INT_PIN 5
#define IO2_INT_PIN 4

#define BOOT_MODE_PIN 8