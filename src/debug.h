#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG 

#ifdef DEBUG
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debuglnB(x) Serial.println(x, BIN)
#else
#define debug(x)
#define debugln(x)
#define debuglnB(x) 
#endif   






#endif