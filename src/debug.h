#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG 

#ifdef DEBUG
#define debug(x) Serial1.print(x)
#define debugln(x) Serial1.println(x)
#define debuglnB(x) Serial1.println(x, BIN)
#define debugWrite(x) Serial1.write(x)
#define debuglnD(x) Serial1.println(x, DEC)
#define debugBegin(x) Serial1.begin(x)
#else
#define debug(x)
#define debugln(x)
#define debuglnB(x)
#define debugWrite(x)
#define debugDEC(x)
#define debugBegin(x) 
#endif   






#endif