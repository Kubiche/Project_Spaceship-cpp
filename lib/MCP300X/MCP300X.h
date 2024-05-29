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
  void begin(unsigned char cs);
  unsigned int Read(unsigned char channel);
private:
  // Object variable containing the ADC IC Chip Select pin
  unsigned char adc_cs_;

};

#endif