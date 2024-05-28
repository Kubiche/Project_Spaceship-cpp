#include "MAX72XX.h"
#include <spi.h>

/**
 * @brief sets up the Chip Select pin to HIGH and sends a "Display Test" for 1 second
 * 
 * @param cs Chip Select pin of the device or cascaded devices
 * @param devices the number of devices
 */
 void MAX72XX::begin(int cs, uint8_t devices)
{
  led_cs_ = cs;
  number_of_devices_ = devices;  
  pinMode(led_cs_, OUTPUT); // Set the CS pin as output
  digitalWrite(led_cs_, HIGH); // Set CS pin to High  
  for (uint8_t i = 0; i < number_of_devices_; i++)
  {    
    setRegister(i, OP_SHUTDOWN, 1); // Turn LED controller on
    setRegister(i, OP_SCANLIMIT, 7); // set to scan all digit_s
    setRegister(i, OP_INTENSITY, 2); // Set intensity to 2 of 16
    setRegister(i, OP_DISPLAYTEST, 1);
    delay(1000);    
    setRegister(i, OP_DISPLAYTEST, 0);
  }  
}

/**
 * @brief Sets a register in the MAX7219 or MAX7221 device
 * 
 * @param device The device number 0-N  
 * @param opcode The address of the register to set
 * @param val The value to set in the register
 */
void MAX72XX::setRegister(uint8_t device, uint16_t opcode, uint16_t val) 
{
  uint16_t led_buffer[number_of_devices_] = {0};
  led_buffer[device] = opcode;
  led_buffer[device] = (led_buffer[device] << 8);
  led_buffer[device] |= val;   
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));    
  for (uint8_t i = 0; i < number_of_devices_; i++)
  {
    digitalWrite(led_cs_, LOW);
    SPI.transfer16(led_buffer[i]); //this is the combination of the opcode and the value desired
    digitalWrite(led_cs_, HIGH);
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
void MAX72XX::setLed(uint8_t device, uint8_t dig, uint8_t seg, bool state) 
{
  uint8_t mask = 0b10000000 >> (seg);
  if ((digit_[device][dig] & mask) != state){
    digit_[device][dig] ^= mask; 
    setRegister(device, (dig + 1), digit_[device][dig]);    
  }
}

/**
 * @brief Sets the LED bars on my custom hardware to show a level 0-10
 * 
 * @param device The device number 0-N
 * @param bar the bar number 0-6
 * @param value the value to show 0-10
 */
void MAX72XX::showInBar(uint8_t device, uint8_t bar,uint8_t value)
{ 
  // Bars indexed from left to right from 1 to 6 
  if ((value >= 0) && (value <= 10))
  {
    const uint16_t Kfullbar[6] {0b1111111111000000, 0b0011111111110000, 0b0000111111111100, 0b0000001111111111, 0b1111111111000000, 0b0011111111110000};// Array to store the full bar value to manipulate for the actual value  
    uint8_t bar_top_byte;
    uint8_t bar_bottom_byte;
    uint8_t top_digit_opcode;
    uint8_t bottom_digit_opcode;
    uint8_t index = bar - 1;
    if ( bar < 5)
    {
      bar_top_byte = bar;
      bar_bottom_byte = bar - 1;
      top_digit_opcode = bar + 1;
      bottom_digit_opcode = bar;
    }
    else
    {
      bar_top_byte = bar + 1;
      bar_bottom_byte = bar;
      top_digit_opcode = bar + 2;
      bottom_digit_opcode = bar + 1;
    }    
    uint8_t shifted = (10 - value); // fugure out the amount of bits to shift
    uint16_t barlevel = Kfullbar[index] << shifted;  // shift the bits left to show the desired level. Bar 1 index 0 of array.    
    barlevel &= Kfullbar[index];  // Apply modified mask
    digit_[device][bar_top_byte] &= ~Kfullbar[index]; // Apply LSBits of actual mask to the top byte of the bar 
    digit_[device][bar_top_byte] |= barlevel;     // Apply the LSBits of the value to the top byte of bar
    setRegister(device, top_digit_opcode, digit_[device][bar_top_byte]); // Send the new value of the top byte of the bar to the proper digit
    digit_[device][bar_bottom_byte] &= ~Kfullbar[index] >> 8;      // Apply MSBits of mask to the bottom byte of the bar
    digit_[device][bar_bottom_byte] |= barlevel >> 8;          // Apply the MSBits of the value to the bottom byte of the bar
    setRegister(device, bottom_digit_opcode, digit_[device][bar_bottom_byte]);     //  Send the new value of the bottom byte to the propper digit
  }
}     

/**
 * @brief Sets the desired LED to ON or OFF based on a number 1-64
 * 
 * @param device The device number 0-N
 * @param led_number The LED number 1-64
 * @param state The state ON(TRUE) or OFF(FALSE)
 */
void MAX72XX::setLedByNumber(uint8_t device, int led_number, bool state)
{
  
  setLed(device, ((led_number - 1) / 8), (led_number % 8), state); // divided to get the digit on the MAX72XX (0-7) and modulo used to figure out the segment (0-7) 
} 