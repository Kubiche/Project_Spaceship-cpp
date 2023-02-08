#include "CommandParser.h"
#include "MAX72XX.h"

extern MAX72XX led;

void parseCommand(uint8_t cmd[4])
{
    switch (cmd[0]) // Command type recieved from Rpi
    {
        case 0:  // Test mode. Not implemented yet
            
            break;
            
        case 1:  // Turn on LED command type
            led.SetLed(cmd[1], cmd[2], cmd[3], true);
            break;

        case 2:  // Turn OFF LED command type
            led.SetLed(cmd[1], cmd[2], cmd[3], false);
            break;

        case 3: // Bar graph LED command type
            led.Show_in_bar(cmd[1], cmd[2], cmd[3]);
            break;
        
    } 
}
    