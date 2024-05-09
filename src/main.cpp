#include <Arduino.h>
#include <HID.h>
#include <Wire.h>
#include "Inputs.h"
#include "Defines.h"
#include "MAX72XX.h"


//Instanciation and intialization of the IO chips
MCP23017 io1(IO1_I2C_ADDRESS, IO1_INT_PIN);
MCP23017 io2(IO2_I2C_ADDRESS, IO2_INT_PIN);

//Instanciation and intialization of the Analog to Digital Converter chip
MCP300X adc(9);


MAX72XX led(LED_CS, 2);

// Create the Joystick. Autoupdate set to false in setup()
Joystick_ Joystick(0x03,0x04,
  32, 0,                    //  Button Count, Hat Switch Count
  true, true, true,     //  X and Y and Z Axis
  true, true, true,        //  Rx, Ry, or Rz
  false, true,            //  rudder or throttle
  false, false, false);    //  accelerator, brake, or steering


void setup() 
{  
    
  

  debugln("MAIN WAIT");

   // set up I2c bus
   Wire.begin();
   // set up SPI bus
   SPI.begin();

  // Initiate and set the joystick to manual update to prevent USB overflow
	Joystick.begin(false);  

  
   
  //led.Begin(LED_CS, 2); 

  //------------------------------------------------------Write any test code above here since the while below will halt code---------------------------------------------------------------------------------------------
    
  Serial.begin(115200); // Initialize Serial connection to Raspbery pi
  

  
}

void loop() 
{  
  updateAnalogs();
  updateDigitals();
  Joystick.sendState(); //Send joystick updated states to the PC           
}
