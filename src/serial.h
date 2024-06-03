#ifndef SERIAL_H_
#define SERIAL_H_

void getSerialCommand();
unsigned char parseSerial(unsigned char (&incoming)[3], unsigned char length);
void decodeCommand(unsigned char (&command)[3]);
unsigned char convertChar(unsigned char character);





#endif