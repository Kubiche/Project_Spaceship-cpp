/*
This code is still under construction.
*/
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