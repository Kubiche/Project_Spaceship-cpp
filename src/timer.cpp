#include "timer.h"

/**
 * @brief Construct a new TIMER::TIMER object
 * 
 * @param interval time interval for timer in mili-seconds.
 */
TIMER::TIMER(int interval)
{
    m_interval = interval;
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
    m_last_triggered = millis();    
}

/**
 * @brief Checks if the interval of time has elapsed
 * 
 * @return true If the interval has passed.
 * @return false If the interval has not passed.
 */
bool TIMER::check()
{
    if ((millis() - m_last_triggered) > m_interval)
    {
        resetTimer();
        return true;
    }
    else
    {        
        return false;
    }
}