#include "MAX72XX.h"
#include <spi.h>

/**
 * @brief sets up the Chip Select pin to HIGH and sends a "Display Test" for 1 second
 * 
 * @param cs Chip Select pin of the device or cascaded devices
 * @param devices the number of devices 1-N
 */
void MAX72XX::begin(unsigned char cs, unsigned char devices)
{
    _led_cs = cs;
    _number_of_devices = devices;  
    pinMode(_led_cs, OUTPUT); // Set the CS pin as output
    digitalWrite(_led_cs, HIGH); // Set CS pin to High  
    for (unsigned char i = 0; i < _number_of_devices; i++)
    {      
        setRegister(i, OP_SHUTDOWN, 1); // Turn LED controller on
        setRegister(i, OP_SCANLIMIT, 7); // set to scan all _digits
        setRegister(i, OP_INTENSITY, 8); // Set intensity 0-16        
    }  
}

/**
 * @brief Sets a register in the MAX7219 or MAX7221 device
 * 
 * @param device The device number 0-N  
 * @param opcode The address of the register to set
 * @param val The value to set in the register
 */
void MAX72XX::setRegister(unsigned char device, uint16_t opcode, uint16_t val) 
{
    uint16_t led_buffer[_number_of_devices] = {OP_NOOP};
    led_buffer[device] = opcode;
    led_buffer[device] = (led_buffer[device] << 8);
    led_buffer[device] |= val;   
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));    
    for (unsigned char i = 0; i < _number_of_devices; i++)
    {
        digitalWrite(_led_cs, LOW);
        SPI.transfer16(led_buffer[i]); //this is the combination of the opcode and the value desired
        digitalWrite(_led_cs, HIGH);
    }  
    SPI.endTransaction();
}

/**
 * @brief This function sets an led to ON or OFF based on the device, digit and segment it's tied to.
 * 
 * @param device The device number 0-N
 * @param dig The digit number 0-7
 * @param seg The segment number 0(DP)-7(G)
 * @param state The state, ON(TRUE) or OFF(FALSE)
 */
void MAX72XX::setLed(unsigned char device, unsigned char dig, unsigned char seg, bool state) 
{
    unsigned char mask = 0b10000000 >> (seg);
    bool led_state = (_digit[device][dig] & mask);
    if ( led_state != state)
    {
        _digit[device][dig] ^= mask; 
        setRegister(device, (dig + 1), _digit[device][dig]);    
    }
}

/**
 * @brief Sets the LED bars on my custom hardware to show a level 0-10
 * 
 * @param device The device number 0-N
 * @param bar the bar number 0-6
 * @param value the value to show 0-10
 */
void MAX72XX::showInBar(unsigned char device, unsigned char bar,unsigned char value)
{ 
    // Bars indexed from left to right from 1 to 6 
    if ((value >= 0) && (value <= 10))
    {
        const uint16_t Kfullbar[6] {0b1111111111000000, 0b0011111111110000, 0b0000111111111100, 0b0000001111111111, 0b1111111111000000, 0b0011111111110000};// Array to store the full bar value to manipulate for the actual value  
        unsigned char bar_top_byte;
        unsigned char bar_bottom_byte;
        unsigned char top__digitopcode;
        unsigned char bottom__digitopcode;
        unsigned char index = bar - 1;
        if ( bar < 5)
        {
            bar_top_byte = bar;
            bar_bottom_byte = bar - 1;
            top__digitopcode = bar + 1;
            bottom__digitopcode = bar;
        }
        else
        {
            bar_top_byte = bar + 1;
            bar_bottom_byte = bar;
            top__digitopcode = bar + 2;
            bottom__digitopcode = bar + 1;
        }    
        unsigned char shifted = (10 - value); // fugure out the amount of bits to shift
        uint16_t barlevel = Kfullbar[index] << shifted;  // shift the bits left to show the desired level. Bar 1 index 0 of array.    
        barlevel &= Kfullbar[index];  // Apply modified mask
        _digit[device][bar_top_byte] &= ~Kfullbar[index]; // Apply LSBits of actual mask to the top byte of the bar 
        _digit[device][bar_top_byte] |= barlevel;     // Apply the LSBits of the value to the top byte of bar
        setRegister(device, top__digitopcode, _digit[device][bar_top_byte]); // Send the new value of the top byte of the bar to the proper digit
        _digit[device][bar_bottom_byte] &= ~Kfullbar[index] >> 8;      // Apply MSBits of mask to the bottom byte of the bar
        _digit[device][bar_bottom_byte] |= barlevel >> 8;          // Apply the MSBits of the value to the bottom byte of the bar
        setRegister(device, bottom__digitopcode, _digit[device][bar_bottom_byte]);     //  Send the new value of the bottom byte to the propper digit
    }
}     

/**
 * @brief Sets the desired LED to ON or OFF based on a number 1-64
 * 
 * @param device The device number 0-N
 * @param led_number The LED number 1-64
 * @param state The state ON(TRUE) or OFF(FALSE)
 */
void MAX72XX::setLedByNumber(unsigned char device, unsigned char led_number, bool state)
{
    // Zero based the number for the MAX72XX device
    unsigned char zero_based_number = (led_number - 1);
    // divided to get the digit on the MAX72XX (0-7) and modulo used to figure out the segment (0-7) 
    setLed(device, (zero_based_number / 8), (zero_based_number % 8), state); 
}

/**
 * @brief Set the MAX72XX's in Display Mode  for a time in seconds
 * 
 * @param duration time in seconds the test should last. This is a blocking function.
 */
void MAX72XX::displayTest(unsigned char duration)
{
    for (unsigned char i = 0; i < _number_of_devices; i++)
    {
        setRegister(i, OP_DISPLAYTEST, 1);
    }    
    delay(duration * 1000);
    for (unsigned char i = 0; i < _number_of_devices; i++)
    {
        setRegister(i, OP_DISPLAYTEST, 0);
    }    
}