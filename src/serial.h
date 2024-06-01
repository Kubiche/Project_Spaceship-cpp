#ifndef SERIAL_H_
#define SERIAL_H_

void getSerialCommand();
void parseSerial(unsigned char (&incoming)[6]);
void decodeCommand(unsigned char (&command)[3]);





#endif