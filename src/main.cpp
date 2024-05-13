#include <Arduino.h>
#include "IO.h"
#include "debug.h"


void setup() 
{ 
  initIO();
}

void loop() 
{   
  getIO();
  pushIO();      
}