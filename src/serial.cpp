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
        delay(5); // allow the whole command to come in
        unsigned char incoming = Serial.available();        
        unsigned char serial_buffer[incoming] = {0};
        Serial.readBytesUntil('\n', serial_buffer, incoming);
        parseSerial(serial_buffer, incoming);       
    }
}

void parseSerial(unsigned char (&incoming)[], unsigned char length)
{
    enum : unsigned char 
    {
        Display_Test = 0,
        LED_Bar = 1,
        LED = 2
    };
    if (incoming[0] == Display_Test)
    {
        led.setRegister
    }    
}