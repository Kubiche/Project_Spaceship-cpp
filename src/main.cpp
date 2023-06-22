#include <Arduino.h>
#include <HID.h>
#include <Wire.h>
#include "Inputs.h"
#include "Defines.h"
#include "MAX72XX.h"
#include "CommandParser.h"

extern MCP23017 io1;
extern MCP23017 io2;

extern MCP300X adc;


MAX72XX led;

extern Joystick_ Joystick;

void setup() 
{  
   Serial.begin(115200); 
  

  debugln("MAIN WAIT");

   // set up I2c bus
   Wire.begin();
   // set up SPI bus
   SPI.begin();

  // Initiate and set the joystick to manual update to prevent USB overflow
	Joystick.begin(false);  

  // MCP23017 IO expanders  
  io1.Begin(IO1_I2C_ADDRESS, IO1_INT_PIN);
  io2.Begin(IO2_I2C_ADDRESS, IO2_INT_PIN);
  adc.Begin(9);
  led.Begin(LED_CS, 2); 

  //------------------------------------------------------Write any test code above here since the while below will halt code---------------------------------------------------------------------------------------------
    
  Serial.begin(115200); // Initialize Serial connection to Raspbery pi
  Serial.setTimeout(1000);

  
}

void loop() 
{  
  updateAnalogs();
  updateDigitals();
  Joystick.sendState(); //Send joystick updated states to the PC
  if (Serial.available())
  {
      uint8_t command[4];
      if (Serial.readBytesUntil('\n', command, 4))
      {
          parseCommand(command);
      }   
  
  }

           
}
