#include "MCP300X.h"
#include <spi.h>

/**
 * @brief Sets the objects chip select to HIGH
 * 
 * @param cs Chip Select pin for the MCP300X IC device
 */
void MCP300X::begin(uint8_t cs)
{ 
    m_adc_cs = cs;  
    pinMode(m_adc_cs, OUTPUT);
    digitalWrite(m_adc_cs, HIGH);
}

/**
 * @brief Gets the analog to digital converted reads from the MCP300X device
 * 
 * @param channel The channel to read on the device 0-7
 * @return unsigned int containing the read
 */
unsigned int MCP300X::read(uint8_t channel)
{
    //stores data temporarily to transmit to IC and stores received readings to return
    uint8_t buffer[3]; 
    //this is the start bit as per datasheet section 6.1
    buffer[0] = 0b00000001; 
    //harcoded 1 for single ended reading and channel number combined
    buffer[1] = 0b10000000 | (channel << 4); 
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(m_adc_cs, LOW);
    SPI.transfer(buffer, 3); 
    digitalWrite(m_adc_cs, HIGH);
    SPI.endTransaction();
    //combine the relevant bits to come up with the actual reading
    unsigned int read = ((buffer[1] & 0b00000011) << 8) | buffer[2]; 
    return read;
}