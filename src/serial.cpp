#include <Arduino.h>
#include "serial.h"
#include "debug.h"
#include "MAX72XX.h"
#include "debug.h"

extern MAX72XX led;

void getSerialCommand()
{
    if (Serial.available())
    {
        debug("Serial available: ");
        debugln(Serial.available());
        unsigned char serial_command[6] = {0}; // command to hold the commands as they come from the serial interface.       
        delay(5); // Delay to allow all the data to come in        
        while (Serial.available() > 0)
        {      
            for (unsigned char i = 0; i < 6; i++)
            {
                serial_command[i] = Serial.read();
            }
            Serial.read();
        }
        parseSerial(serial_command);
    }
}

/**
 * @brief Serial parser
 * 
 * @param incoming Array containing the whole serial input
 */
void parseSerial(unsigned char (&incoming)[6])
{
    unsigned char command[3] = {0};
    unsigned char index = 0;
    for (unsigned char i = 0; i < 6; i++)
    {
        if (incoming[i] == '\n')
        {
            decodeCommand(command);
            return;
        }
        else if (incoming[i] == ',')
        {
            index = index + 1;    
        }
        else
        {
            command[index] = incoming[i];
        }
    }
}

/**
 * @brief 
 * 
 * @param command array passed by reference containing the commands
 */
void decodeCommand(unsigned char (&command)[3])
{
    debugln("Decoding Command");
    debugWrite(command, 3);
    enum : unsigned char {Display_Test = 0, LED_Bar = 1, LED = 2 };
    if (command[0] == Display_Test) 
    {
        for (unsigned char i = 0; i <= 1; i++)
        {
            led.setRegister(i, OP_DISPLAYTEST, command[2]);      
        }
    }
    if (command[0] == LED_Bar)
    {
        led.showInBar(2, command[1], command[2]);
    }
    if (command[0] == LED)
    {
        led.setLedByNumber(1, command[1], command[2]);
    }
}