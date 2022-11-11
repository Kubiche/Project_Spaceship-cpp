#include <Arduino.h>
#include <KerbalSimpit.h>
#include <HID.h>
#include <Wire.h>
#include "inboundMessages.h"
#include "Inputs.h"
#include "Debug.h"
#include "Defines.h"
#include <MAX72XX.h>

extern MCP23017 io1;
extern MCP23017 io2;

extern MCP300X adc;

KerbalSimpit mySimpit(Serial);
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
  led.Begin(LED_CS);

  /*
  for (int i = 1; i < 9; i++)
  {
    led.setLedReg(1, i, 0);
    led.setLedReg(0, i, 0);
  }
  for (int lev = 10; lev >= 0; lev--)
  {
  for (int i = 1; i < 7; i++)
  {
   led.show_in_bar(1, i, lev);
   delay(1000);      
  }
  }
  */
  

  //------------------------------------------------------Write any test code above here since the while below will halt code---------------------------------------------------------------------------------------------
    
  Serial.begin(115200); // Initialize Serial connection to mod  
  pinMode(BOOT_MODE_PIN, INPUT_PULLUP);  
  if (digitalRead(BOOT_MODE_PIN))
  {
    while (!mySimpit.init()) 
    {
      delay(100);
    }    
  }
  else
  {
    debugln("MOD connection bypassed");
  }
  
  mySimpit.inboundHandler(messageHandler); // callback function  
  // Register to desired messages from simpit
  mySimpit.registerChannel(SF_STAGE_MESSAGE);
  mySimpit.registerChannel(LF_STAGE_MESSAGE);
  mySimpit.registerChannel(MONO_MESSAGE);
  mySimpit.registerChannel(ELECTRIC_MESSAGE);
  mySimpit.registerChannel(ACTIONSTATUS_MESSAGE);   

}

void loop() 
{   
  mySimpit.update(); // Update messages from simpit mod, as part of it the function messageHandler gets called to process the mod's output in our code (see inboundMessages.h)
  updateAnalogs();
  updateDigitals();
  Joystick.sendState(); //Send joystick updated states to the PC      
}
Footer
© 2022 GitHub, Inc.
Footer navigation
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
