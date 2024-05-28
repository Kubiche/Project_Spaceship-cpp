#include "IO.h"
#include "debug.h"



MCP23017 io1;
MCP23017 io2;
MCP300X adc;
MAX72XX led;

// Variable to store the time of the last analog value read.
unsigned long analog_last_read = 0; 

/**
 * @brief The Joystick object with 32 buttons 6 axis and a Throttle
 * 
 */
Joystick_ Joystick(0x05,0x04,
  32, 0,                    //  Button Count, Hat Switch Count
  true, true, true,     //  X and Y and Z Axis
  true, true, true,        //  Rx, Ry, or Rz
  false, true,            //  rudder or throttle
  false, false, false);    //  accelerator, brake, or steering

/**
 * @brief Sets up the Input/Output devices 
 * 
 */
void initIO()
{
  Wire.begin();
  SPI.begin();
  Serial.begin(115200);  
  io1.begin(IO1_I2C_ADDRESS, IO1_INT_PIN);
  io2.begin(IO2_I2C_ADDRESS, IO2_INT_PIN);
  adc.begin(ADC_CS_PIN);
  led.begin(LED_CS, LED_DEV_COUNT);
  Joystick.begin(false);
}

/**
 * @brief Gets the Analog reads from the ADC at defined intervals
 * 
 */
void updateAnalogs()   
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

/**
 * @brief Check if the interrup flags from the MCP23017 is set, reads the interrupt flag and gets the GPIO values.
 * 
 */
void updateDigitals()
{
  if (!digitalRead(IO1_INT_PIN)) //check for interrupt pin from io expander chip to see if any changes in states.
  {
    unsigned int flags = io1.ReadIntFlag(); //read the interrupt flags to see which pins trigerred the interrupt
    unsigned int inputs = io1.ReadGPIOs();  //read the states of the gpio pins    
    for (unsigned int i = 0; i < 16; i++)
    {      
      unsigned int mask = (1 << i);      
      if (flags & mask)
      {        
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
    for (unsigned int i = 0; i < 16; i++)
    {      
      unsigned int mask = (1 << i);      
      if (flags & mask)
      {        
        bool state = inputs & mask; //cast the bitwise and of the flagged bit and the read bit into a boolean to acomodate the byte allowed by the library function.
        Joystick.setButton((i + 16), state); //update the state of the button associated with the pin/s that trigerred the interrupt        
      } 
    }    
  }
}

/**
 * @brief check if the serial buffer contains a command and if so, reads the bytes until the termination char (/n)
 * 
 */
void getSerialCommand()
{
  if (Serial.available())
  {
    uint8_t command[3] = {0};
    uint8_t index = 0;
    delay(5); // Delay to allow all the data to come in    
    while (Serial.available() > 0)
    {      
      uint8_t charIn = Serial.read();      
      if (charIn == '\n') // If received the terminator character, decode the command
      {
        while (Serial.available() > 0 ) // If buffer still holds data
        {
          Serial.read(); // Empty the buffer
        }
        debug("CMD: ");
        debug(command[0]);
        debug(",");
        debug(command[1]);
        debug(",");
        debugln(command[2]);
        decodeCommand(command[0], command[1], command[2]);
      }
      else if (charIn == ',') // If recieved decimal 44 (",") used as data separator, ignore and increase the index
      {
        index = index + 1;
      }
      else
      {
        command[index] = charIn - '0'; // get the actual number sent, not just the ASCII code.
      }

    }
  }
}

/**
 * @brief Decodes the command to issue to the peripherals
 * 
 * @param command_type The command type 0: Display-Test | 1: Show on LED bar | 2: Control LED
 * @param command LED number, Bar number to control
 * @param value ON(True)/OFF(False) or 0-10 for led-bars
 */
void decodeCommand(uint8_t command_type, uint8_t command, uint8_t value)
{
  if (command_type == 0) 
  {
    for (int i = 0; i <= LED_DEV_COUNT; i++)
    {
      led.setRegister(i, OP_DISPLAYTEST, value);      
    }
  }
  if (command_type == 2)
  {
    led.setLedByNumber(1, command, value);
  }
}

/**
 * @brief Gets the Input/Output from peripherals
 * 
 */
void getIO()
{
  updateAnalogs();
  updateDigitals();
  getSerialCommand();
}

/**
 * @brief Updates the input changes to the computer
 * 
 */
void pushIO()
{
  Joystick.sendState();
}
