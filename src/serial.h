#ifndef SERIAL_H_
#define SERIAL_H_

void getSerialCommand();
void parseSerial(unsigned char (&incoming)[], unsigned char length);
void decodeCommand(unsigned char (&command)[], unsigned char length);





#endif