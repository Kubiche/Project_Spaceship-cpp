#pragma once
#include "Arduino.h"
#include "MAX72XX.h"


#define MESSAGE_LENGTH 4

void parseCommand(uint8_t message[]);
