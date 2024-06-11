#ifndef MCP300X_H_
#define MCP300X_H_

#include <Arduino.h>

/**
 * @brief Class for the Analog to Digital converter MCP300X IC
 * 
 */
class MCP300X
{
public:
    void begin(uint8_t cs);
    unsigned int read(uint8_t channel);
private:
    // Object variable containing the ADC IC Chip Select pin
    uint8_t m_adc_cs;

};

#endif