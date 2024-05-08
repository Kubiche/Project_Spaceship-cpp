#include "Inputs.h"
#include "Defines.h"




extern MCP23017 io1;
extern MCP23017 io2;

extern MCP300X adc;

unsigned long analog_last_read = 0; // variable to store the time of the last analog value read.

extern Joystick_ Joystick;

void updateAnalogs()   //read analog values and update accordingly.
{  
  if ((millis() - analog_last_read) > ANALOG_CHECK_INTERVAL)
  {
    unsigned int channel[8];
    for (uint8_t i = 0; i < 7; i++)
    {
      channel[i] = adc.Read(i); //Read all channels of the ADC IC and store it in the array
      //debug("channel");
      //debug(i);
      //debug(": ");
      //debugln(channel[i]);
    }
    analog_last_read = millis();    
    Joystick.setXAxis(channel[0]);
    Joystick.setYAxis(channel[1]);
    Joystick.setZAxis(channel[2]);
    //Joystick.setRxAxis(channel[3]);  
    //Joystick.setRyAxis(channel[4]);
    //Joystick.setRzAxis(channel[5]);
    Joystick.setThrottle(channel[6]);  
  }    
}

void updateDigitals()
{
  if (!digitalRead(IO1_INT_PIN)) //check for interrupt pin from io expander chip to see if any changes in states.
  {
    unsigned int flags = io1.ReadIntFlag(); //read the interrupt flags to see which pin/s trigerred the interrupt
    unsigned int inputs = io1.ReadGPIOs();  //read the states of the gpio pins
    debug("Flags : ");
    debuglnB(flags);
    debug("inputs: ");
    debuglnB(inputs);
    for (unsigned int i = 0; i < 16; i++)
    {      
      unsigned int mask = (1 << i);      
      if (flags & mask)
      {
        debug("i: ");
        debugln(i);
        debug("mask: ");
        debuglnB(mask);
        bool state = inputs & mask; //cast the bitwise and of the flagged bit and the read bit into a boolean to acomodate the byte allowed by the library function.
        Joystick.setButton(i, state); //update the state of the button associated with the pin/s that trigerred the interrupt
        debug("state: ");
        debugln(state);
      } 
    }    
  }

  if (!digitalRead(IO2_INT_PIN)) //check for interrupt pin from io expander chip to see if any changes in states.
  {
    unsigned int flags = io2.ReadIntFlag(); //read the interrupt flags to see which pin/s trigerred the interrupt
    unsigned int inputs = io2.ReadGPIOs();  //read the states of the gpio pins
    debug("Flags : ");
    debuglnB(flags);
    debug("inputs: ");
    debuglnB(inputs);
    for (unsigned int i = 0; i < 16; i++)
    {      
      unsigned int mask = (1 << i);      
      if (flags & mask)
      {
        debug("i: ");
        debugln((i+16));
        debug("mask: ");
        debuglnB(mask);
        bool state = inputs & mask; //cast the bitwise and of the flagged bit and the read bit into a boolean to acomodate the byte allowed by the library function.
        Joystick.setButton((i + 16), state); //update the state of the button associated with the pin/s that trigerred the interrupt
        debug("state: ");
        debugln(state);
      } 
    }    
  }
}

void getSerial()
{
  uint8_t serialIn = 0;
  uint8_t parsedCommand[4] = {0};
  uint8_t commandIndex = 0;
  if (Serial.available())
  {
    delay(5);
    while (Serial.available() > 0)
    {
      serialIn = Serial.read();
      if serialIn == "\n";
      {
        //place holder for actual command handling
      }
      else
      {
        if (serialIn == ",")
        {
          commandIndex ++;
        }
      }
      else
      {
        parsedCommand[commandIndex] = serialIn;
      }
    }
  }
}