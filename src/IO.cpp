#include "IO.h"
#include "debug.h"



MCP23017 io1;
MCP23017 io2;
MCP300X adc;
MAX72XX led;


// Variable to store the time of the last analog value read.
unsigned long analog_last_read = 0;
unsigned long joystick_last_push = 0;

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
    for (unsigned char i = 0; i < 7; i++)
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
    debugln("IO1 interrupt triggered");
    unsigned int flags = io1.ReadIntFlag(); //read the interrupt flags to see which pins trigerred the interrupt
    unsigned int inputs = io1.ReadGPIOs();  //read the states of the gpio pins    
    for (unsigned int i = 0; i < 16; i++)
    {      
      unsigned int mask = (1 << i);      
      if (flags & mask)
      {        
        bool state = inputs & mask; //cast the bitwise and of the flagged bit and the read bit into a boolean to acomodate the byte allowed by the library function.
        Joystick.setButton(i, state); //update the state of the button associated with the pin/s that trigerred the interrupt        
      } 
    }    
  }
  if (!digitalRead(IO2_INT_PIN)) //check for interrupt pin from io expander chip to see if any changes in states.
  {
    debugln("IO2 interrupt triggered");
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
    debug("Serial available: ");
    debugln(Serial.available());
    unsigned char command_buffer[3] = {0}; // Buffer to hold the commands as they come from the serial interface.
    unsigned char char_in = 0;    
    unsigned char index = 0;
    for (unsigned char i = 0; i < (sizeof(command_buffer) / sizeof(command_buffer[0])); i++) 
    {
      command_buffer[i] = 0; // zero out the buffer
    }
    delay(5); // Delay to allow all the data to come in 
    debug("buffer 0 : ");
    debugln(command_buffer[0]);
    debug("buffer 1 : ");
    debugln(command_buffer[1]);
    debug("buffer 2 : ");
    debugln(command_buffer[2]);   
    while (Serial.available() > 0)
    {      
      char_in = Serial.read();
      //debuglnD(char_in);
      //debugln(char_in);      
      if (char_in == '\n') // If received the terminator character, decode the command
      {        
        debug("CMD: ");
        debug(command_buffer[0]);
        debug(",");
        debug(command_buffer[1]);
        debug(",");
        debug(command_buffer[2]);
        debugln();
        if (index == ((sizeof(command_buffer) / sizeof(command_buffer[0])) - 1)) // crude way of checking the command length
        {  
          decodeCommand(command_buffer);
        }
        else
        {
          debugln("Null Command");
        }                
      }
      else if (char_in == ',') // If recieved decimal 44 (",") used as data separator, ignore and increase the index
      {
        index++;
      }
      else
      {        
        unsigned char zeroed_char = char_in - '0'; // get the actual number sent, not just the ASCII code.
        debug("zeroed char: ");
        debugln(zeroed_char);
        command_buffer[index] = zeroed_char; 
        debug("New buffer value: ");
        debugln(command_buffer[index]);        
      }

    }
  }
}

/**
 * @brief 
 * 
 * @param buffer Buffer passed by reference containing the commands
 */
void decodeCommand(unsigned char (&buffer)[3])
{
  debugln("Decoding Command");
  enum : unsigned char {Display_Test = 0, LED_Bar = 1, LED = 2 };
  if (buffer[0] == Display_Test) 
  {
    for (unsigned char i = 0; i <= LED_DEV_COUNT; i++)
    {
      led.setRegister(i, OP_DISPLAYTEST, buffer[2]);      
    }
  }
  if (buffer[0] == LED_Bar)
  {
    led.showInBar(2, buffer[1], buffer[2]);
  }
  if (buffer[0] == LED)
  {
    led.setLedByNumber(1, buffer[1], buffer[2]);
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
  if ((millis() - joystick_last_push) > JOYSTICK_UPDATE_INTERVAL)
  {
  Joystick.sendState();
  joystick_last_push = millis();
  }
}
