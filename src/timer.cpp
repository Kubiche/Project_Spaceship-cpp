#include "timer.h"

void TIMER::start(int interval)
{
    m_interval = interval;
    resetTimer();
}


void TIMER::resetTimer()
{
    m_last_triggered = millis();    
}

bool TIMER::check()
{
    if ((millis() - m_last_triggered) > m_interval)
    {
        return true;
    }
    else
    {
        resetTimer();
        return false;
    }
}