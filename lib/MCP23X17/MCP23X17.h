#ifndef MCP23X17_H_
#define MCP23X17_H_

#include <Arduino.h>




//////////////////////////////////////////////////////////////////Registers/////////////////////////////////////////////////////////
//!< I/O direction register
#define MCP23017_IODIRA 0x00   
//!< I/O direction register
#define MCP23017_IODIRB 0x01   
//!< Input polarity register
#define MCP23017_IPOLA 0x02    
//!< Input polarity register
#define MCP23017_IPOLB 0x03    
//!< Interrupt-on-change control register
#define MCP23017_GPINTENA 0x04 
//!< Interrupt-on-change control register
#define MCP23017_GPINTENB 0x05 
//!< Default compare register for interrupt-on-change
#define MCP23017_INTCONA 0x08 
//!< Interrupt control register
#define MCP23017_INTCONB 0x09 
//!< Configuration register
#define MCP23017_IOCON 0x0A  
//!< Pull-up resistor configuration register for port A
#define MCP23017_GPPUA 0x0C   
//!< Pull-up resistor configuration register for port B
#define MCP23017_GPPUB 0x0D    
//!< Interrupt capture register for port A
#define MCP23017_INTCAPA 0x10 
//!< Interrupt capture register for port B
#define MCP23017_INTCAPB 0x11 
//!< interrup pin capture register for port A
#define MCP23017_INTFA 0x0E   
//!< interrup pin capture register for port B
#define MCP23017_INTFB 0x0F   
//!< Port-A register
#define MCP23017_GPIOA 0x12   
//!< Port-B register
#define MCP23017_GPIOB 0x13   
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

          
/**
 * @brief Class for Input/Output MCP23017 IC devices
 * 
 */
class MCP23017 
{
public:
    void begin(uint8_t I2C_address, uint8_t interrupt_pin); //class constructor
    unsigned int ReadGPIOs(); // Reads gpio registers from IC and stores them in the array bellow.
    unsigned int ReadIntFlag();    
private:
    int _device_address;
    int _interrupt_pin;    
};

#endif