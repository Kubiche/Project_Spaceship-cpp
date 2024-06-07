#include <Arduino.h>
#include "serial.h"
#include "debug.h"
#include "MAX72XX.h"
#include "debug.h"

extern MAX72XX led;

#define LED_BAR_DEVICE 0
#define LED_DEVICE 1

/**
 * @brief Get the Serial Commands and process them for use
 * 
 */
void getSerialCommand()
{
    if (Serial.available())
    {
        delay(5); // allow the whole command to come in
        bool failed = false;
        uint8_t command_buffer[3] = {0}; // Holds the commands to be decoded into actions
        uint8_t incoming_buffer[3] = {0}; // Holds the incoming data per section
        uint8_t command_length = 0; // holds the amount of bytes (digits) received in the sections
        command_length = Serial.readBytesUntil(',', incoming_buffer, 3); // get the data and store the count of bytes
        if (command_length > 0)
        {
            debug("CMD: ");
            command_buffer[0] = parseSerial(incoming_buffer, command_length); // Store the parsed message into the command buffer to be decoded.
            if (command_buffer[0] == 255)
            {
                failed = true;
            }
            debug(command_buffer[0]);
            debug(" ,");
        }
        else
        {
            failed = true;            
        }
        command_length = Serial.readBytesUntil(',', incoming_buffer, 3); // get the data and store the count of bytes
        if (command_length > 0)
        {
            command_buffer[1] = parseSerial(incoming_buffer, command_length); // Store the parsed message into the command buffer to be decoded.
            if (command_buffer[1] == 255)
            {
                failed = true;
            }
            debug(command_buffer[1]);
            debug(" ,");
        }
        else
        {
            failed = true;            
        }
        command_length = Serial.readBytesUntil('\n', incoming_buffer, 3); // get the data and store the count of bytes
        if (command_length > 0)
        {
            command_buffer[2] = parseSerial(incoming_buffer, command_length); // Store the parsed message into the command buffer to be decoded.
            if (command_buffer[2] == 255)
            {
                failed = true;
            }
            debug(command_buffer[2]);
            debugln();
        }
        else
        {
            failed = true;            
        }
        if (failed == false)
        {
            decodeCommand(command_buffer); 
        }
        else
        {
            debugln("Invalid Command Message");
        }         
    }
}

/**
 * @brief Parse the incoming bytes from the serial interface into usable commands
 * 
 * @param incoming Data from the serial port no including the separator or terminator char
 * @param length Didits the buffer contains
 * @return uint8_t Parsed Command
 */
uint8_t parseSerial(uint8_t (&incoming)[3], uint8_t length)
{
    uint8_t command = 0;
    if (length == 1)
    {
        command = convertChar(incoming[0]);
        return command;
    }
    if (length == 2)
    {
        command = ((convertChar(incoming[0]) * 10) + convertChar(incoming[1]));
        return command;
    }
    if (length == 3)
    {
        command = (convertChar(incoming[0]) * 100) + (convertChar(incoming[1]) * 10) + convertChar(incoming[2]);
        return command;
    }
    command = 255;
    return command;   
}

/**
 * @brief Decoded te received command to be passed along to the LEDs
 * 
 * @param command Parsed command ready to be used
 */
void decodeCommand(uint8_t (&command)[3])
{
    enum : uint8_t
    {
        DISPLAY_TEST = 0,
        LED_BAR = 1,
        LED = 2
    };
    if (command[0] == DISPLAY_TEST)
    {
        if (command[1] > 0)
        {
            led.displayTest(command[1]);
        }
        else
        {
            led.displayTest();
        }        
    }
    if (command[0] == LED_BAR)
    {
        led.showInBar(LED_BAR_DEVICE, command[1], command[2]);
    }
    if (command[0] == LED)
    {
        if (command[2] == 0)
        {
            led.setLedByNumber(LED_DEVICE,command[1], false);
        }
        if (command[2] == 1)
        {
            led.setLedByNumber(LED_DEVICE,command[1], true);            
        }
        
    }
}

/**
 * @brief Converts an ascii char into an actual number offseted by the ascii 0 (48).
 * 
 * @param character The char to be converted
 * @return uint8_t The converted char
 */
uint8_t convertChar(uint8_t character)
{
    uint8_t adjusted;
    adjusted = character - '0';
    return adjusted;
}