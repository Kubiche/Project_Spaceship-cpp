#ifndef SERIAL_H_
#define SERIAL_H_

void getSerialCommand();
uint8_t parseSerial(uint8_t (&incoming)[3], uint8_t length);
void decodeCommand(uint8_t (&command)[3]);
uint8_t convertChar(uint8_t character);





#endif