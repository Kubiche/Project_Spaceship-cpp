#include "timer.h"

/**
 * @brief Construct a new TIMER::TIMER object
 * 
 * @param interval time interval for timer in mili-seconds.
 */
TIMER::TIMER(int interval)
{
    _interval = interval;
}

/**
 * @brief Starts the timer
 * 
 */
void TIMER::start()
{    
    resetTimer();
}

/**
 * @brief Resets the timer
 * 
 */
void TIMER::resetTimer()
{
    _last_triggered = millis();    
}

/**
 * @brief Checks if the interval of time has elapsed
 * 
 * @return true If the interval has passed.
 * @return false If the interval has not passed.
 */
bool TIMER::check()
{
    if ((millis() - _last_triggered) > _interval)
    {
        resetTimer();
        return true;
    }
    else
    {        
        return false;
    }
}