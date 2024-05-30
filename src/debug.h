#ifndef DEBUG_H_
#define DEBUG_H_

//#define DEBUG 

#ifdef DEBUG
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debuglnB(x) Serial.println(x, BIN)
#define debugWrite(x) Serial.write(x)
#define debugDEC(x) Serial.print(x, DEC)
#else
#define debug(x)
#define debugln(x)
#define debuglnB(x)
#define debugWrite(x)
#define debugDEC(x) 
#endif   






#endif