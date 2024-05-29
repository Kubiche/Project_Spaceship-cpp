/*
This code is still under construction.
*/
#include <Arduino.h>
#include "IO.h"
#include "debug.h"


void setup() 
{ 
  initIO();
  debugln("Setup Complete");  
}

void loop() 
{   
  getIO();
  pushIO();       
}