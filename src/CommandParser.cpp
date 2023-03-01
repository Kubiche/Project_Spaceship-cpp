#include "CommandParser.h"
#include "Arduino.h"

void parseCommand(uint8_t message[])
{
    if (sizeof(message) > MAX_MESSAGE_LENGTH)
    {
        return;
    }

    else 
    {
        uint8_t command_type = message[0];
        uint8_t command_arg_1 = message[1];
        uint8_t command_arg_2 = message[2];

        switch(command_type)
        {
            case 0: // LED test placeholder
                break;

            case 1:  // LED ON 
                
                
                
        } 
    }
}