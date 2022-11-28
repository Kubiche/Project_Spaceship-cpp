#pragma once

#include <Arduino.h>

class MCP300X
{

public:

  void Begin(uint8_t cs);
  unsigned int Read(uint8_t channel);

private:
  uint8_t adc_cs_;

};