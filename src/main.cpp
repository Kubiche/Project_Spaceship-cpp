#include <Arduino.h>
#include "IO.h"


void setup() 
{ 
  initIO();
}

void loop() 
{   
  getIO();
  pushIO();       
}