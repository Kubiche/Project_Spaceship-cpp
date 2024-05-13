#include "MCP300X.h"
#include <spi.h>


void MCP300X::begin(uint8_t cs)
{ 
 adc_cs_ = cs;  
 pinMode(adc_cs_, OUTPUT);
 digitalWrite(adc_cs_, HIGH);
}


unsigned int MCP300X::Read(uint8_t channel)
{
  uint8_t buffer[3]; //stores data temporarily to transmit to IC ans stores received readings to return
  buffer[0] = 0b00000001; //this is the start bit as per datasheet section 6.1
  buffer[1] = 0b10000000 | (channel << 4); //harcoded 1 for single ended reading and channel number combined
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(adc_cs_, LOW);
  SPI.transfer(buffer, 3); 
  digitalWrite(adc_cs_, HIGH);
  SPI.endTransaction();
  unsigned int read = ((buffer[1] & 0b00000011) << 8) | buffer[2]; //combine the relevant bits to come up with the actual reading
  return read;
}