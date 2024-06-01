#include "IO.h"
#include "debug.h"
#include "serial.h"



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
    debugBegin(19200);
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
