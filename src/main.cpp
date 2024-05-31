/*
This code is still under construction.
*/
#include <Arduino.h>
#include "IO.h"
#include "debug.h"


void setup() 
{ 
  debugBegin(19200);
  initIO();    
}

void loop() 
{   
  getIO();
  pushIO();       
}