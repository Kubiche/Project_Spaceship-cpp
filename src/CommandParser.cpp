#include "CommandParser.h"
#include "Arduino.h"
#include "MAX72XX.h"

extern MAX72XX led;

void parseCommand(uint8_t message[])
{
    if (sizeof(message) > MAX_MESSAGE_LENGTH)
    {
        return;
    }

    else 
    {
        uint8_t command_type = message[0];
        uint8_t command_device = message[1];
        uint8_t command_arg_1 = message[2];
        uint8_t command_arg_2 = message[3];
        

        switch(command_type)
        {
            case 0: // LED test placeholder
                break;

            case 1:  // LED ON 
                led.SetLed(command_device, command_arg_1, command_arg_2, true);
                break;
                
            case 2:  // LED OFF 
                led.SetLed(command_device, command_arg_1, command_arg_2, false);
                break;
                
            case 3:  // Show in BAR
                led.Show_in_bar(command_device, command_arg_1, command_arg_2);
                break;
                
                
        } 
    }
}