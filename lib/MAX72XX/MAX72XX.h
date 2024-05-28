#ifndef MAXX72XX_H_
#define MAXX72XX_H_

#include <spi.h>

// Hardcoded limit of devices that can be used.
#define MAX_DEVICES 4

/////////////////////////////////////////////////////////////////MAX72XX Registers////////////////////////////////////////////////////////
//No-Op register address
#define OP_NOOP   0x00
//Digit 0 register address
#define OP_DIGIT0 0x01
//Digit 1 register address
#define OP_DIGIT1 0x02
//Digit 2 register address
#define OP_DIGIT2 0x03
//Digit 3 register address
#define OP_DIGIT3 0x04
//Digit 4 register address
#define OP_DIGIT4 0x05
//Digit 5 register address
#define OP_DIGIT5 0x06
//Digit 6 register address
#define OP_DIGIT6 0x07
//Digit 7 register address
#define OP_DIGIT7 0x08
//Decode Mode register address
#define OP_DECODEMODE  0x09
//Intensity register address
#define OP_INTENSITY   0x0A
//Scan Limit register address
#define OP_SCANLIMIT   0x0B
// Shutdown register address
#define OP_SHUTDOWN    0x0C
// Display Test register address
#define OP_DISPLAYTEST 0x0F
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Class for LED MAX7219 or MAX7221 devices. One object needed for all cascaded devices
 * 
 */
class MAX72XX
{
public:
    void begin(int cs, uint8_t devices);    
    void setRegister(uint8_t device, uint16_t opcode, uint16_t val);  
    void setLed(uint8_t device, uint8_t dig, uint8_t seg, bool state);    
    void showInBar(uint8_t device, uint8_t bar, uint8_t value);    
    void setLedByNumber( uint8_t device, int led_number, bool state);
  
private:
    int led_cs_;
    uint8_t number_of_devices_;
    uint8_t digit_[MAX_DEVICES][8] = {0}; //array to store the max72xx digit values per device.
};      
 
#endif