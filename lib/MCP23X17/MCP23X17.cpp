#include "MCP23X17.h"
#include <Wire.h>

/**
 * @brief Configures the device
 * 
 * @param I2C_address 
 * @param interrupt_pin 
 */
void MCP23017::begin(uint8_t I2C_address, uint8_t interrupt_pin)
{
    device_address_ = I2C_address;
    interrupt_pin_ = interrupt_pin;
    pinMode(interrupt_pin, INPUT_PULLUP);    
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_IOCON);
    Wire.write(0b01111100);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_IODIRA);
    Wire.write(0xFF);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_IODIRB);
    Wire.write(0xFF);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_GPINTENA);
    Wire.write(0xFF);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_GPINTENB);
    Wire.write(0xFF);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_INTCONA);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_INTCONB);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_GPPUA);
    Wire.write(0xFF);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_GPPUB);
    Wire.write(0xFF);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_IPOLA);
    Wire.write(0xFF);
    Wire.endTransmission();
    Wire.beginTransmission(device_address_);
    Wire.write(MCP23017_IPOLB);
    Wire.write(0xFF);
    Wire.endTransmission();
}

/**
 * @brief Reads the state of the GPIO pins
 * 
 * @return unsigned int state of the GPIO pins on port A and B in one. B on the MSB
 */
unsigned int MCP23017::ReadGPIOs() 
{ 
  unsigned int gpio = 0;
  Wire.beginTransmission(device_address_);
  Wire.write(MCP23017_GPIOB);
  Wire.endTransmission();
  Wire.requestFrom(device_address_, 2);
  int i = 0;    
  while (Wire.available())
  {
    gpio = gpio << (8*i);
    gpio = (gpio | Wire.read());
    i++;                
  }
  return gpio;
}

/**
 * @brief Gets the interrupt flag registers value, on which set bits means that a pin caused an interrupt
 * 
 * @return unsigned int containing the interrupt flag registers value with the port B on the MSB
 */
unsigned int MCP23017::ReadIntFlag() 
{
  Wire.beginTransmission(device_address_);
  Wire.write(MCP23017_INTFB);
  Wire.endTransmission();
  Wire.requestFrom(device_address_, 2);
  uint8_t i = 0;
  unsigned int intflag = 0;
  while (Wire.available())
  {
    intflag = intflag << (8*i);
    intflag = (intflag | Wire.read());    
    i++;
  }
  return intflag;
}