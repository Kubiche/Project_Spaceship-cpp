#include "MAX72XX.h"
#include "Debug.h"
#include <spi.h>


 void MAX72XX::Begin(int cs)
{
  led_cs_ = cs;
  number_of_devices_ = LED_Devices;  
  pinMode(led_cs_, OUTPUT); // Set the CS pin as output
  digitalWrite(led_cs_, HIGH); // Set CS pin to High  
  for (uint8_t i = 0; i < number_of_devices_; i++)
  {    
    SetLedReg(i, OP_SHUTDOWN, 1); // Turn LED controller on
    SetLedReg(i, OP_SCANLIMIT, 7); // set to scan all digit_s
    SetLedReg(i, OP_INTENSITY, 2); // Set intensity to 2 of 16
    SetLedReg(i, OP_DISPLAYTEST, 1);
    delay(1000);    
    SetLedReg(i, OP_DISPLAYTEST, 0);
  }  
}

void MAX72XX::SetLedReg(uint8_t device, uint16_t opcode, uint16_t val) 
{
  uint16_t led_buffer[number_of_devices_] = {0};
  led_buffer[device] = opcode;
  led_buffer[device] = (led_buffer[device] << 8);
  led_buffer[device] |= val;   
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));  
  debug("Device 1: ");
  debuglnB(led_buffer[1]);
  debug("Device 0: ");
  debuglnB(led_buffer[0]);
  debugln("sent");  
  for (uint8_t i = 0; i < number_of_devices_; i++)
  {
    digitalWrite(led_cs_, LOW);
    SPI.transfer16(led_buffer[i]); //this is the combination of the opcode and the value desired
    digitalWrite(led_cs_, HIGH);
  }  
  SPI.endTransaction();
}

void MAX72XX::SetLed(uint8_t device, uint8_t dig, uint8_t seg, bool state)
{
  uint8_t mask = 0b10000000 >> (seg-1);
  if ((digit_[device][dig] & mask) != state){
    digit_[device][dig] ^= mask; 
    SetLedReg(device, (dig + 1), digit_[device][dig]);    
  }
}

// This function applies a mask to the byte/digit_ controlling the lower and upper part of each led bar and set a value on it leaving the ones for other bars alone
void MAX72XX::Show_in_bar(uint8_t device, uint8_t bar,uint8_t value)
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
    SetLedReg(device, top_digit_opcode, digit_[device][bar_top_byte]); // Send the new value of the top byte of the bar to the proper digit
    digit_[device][bar_bottom_byte] &= ~Kfullbar[index] >> 8;      // Apply MSBits of mask to the bottom byte of the bar
    digit_[device][bar_bottom_byte] |= barlevel >> 8;          // Apply the MSBits of the value to the bottom byte of the bar
    SetLedReg(device, bottom_digit_opcode, digit_[device][bar_bottom_byte]);     //  Send the new value of the bottom byte to the propper digit
  }
}     
    

