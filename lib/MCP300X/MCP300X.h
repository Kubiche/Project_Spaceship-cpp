#ifndef MCP300X_H_
#define MCP300X_H_

#include <Arduino.h>

class MCP300X
{

public:

  void Begin(uint8_t cs);
  unsigned int Read(uint8_t channel);

private:
  uint8_t adc_cs_;

};

#endif