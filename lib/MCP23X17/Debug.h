#pragma once

#define DEBUG 0

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debuglnB(x) Serial.println(x, BIN)
#else
#define debug(x)
#define debugln(x)
#define debuglnB(x) 
#endif                                            