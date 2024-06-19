#ifndef timer_h_
#define timer_h_

#include <Arduino.h>

/**
 * @brief Simple timer class for non-blocking timed tasks.
 * 
 */
class TIMER
{
private:
    uint64_t m_last_triggered;
    uint8_t m_interval;
    void resetTimer();    

public:
    TIMER(int interval);    
    bool check();
    void start();
};
























#endif