
#include "timer.h"

const DWORD std_interval = 5000;

// **********************
// Class: Timer
// **********************

Timer::Timer()
{
    m_lasttick = GetTickCount();
    m_interval = std_interval;
}

Timer::Timer(DWORD interval)
{
    m_lasttick = GetTickCount();
    m_interval = interval;
}

void Timer::Reset()
{
    m_lasttick = GetTickCount();
}

int Timer::CheckTick()            // Gives number of ticks since last Start(), last C'tor() or last CheckTick()
{
    DWORD now = GetTickCount();

    signed int elapsed = now - m_lasttick;  // can be negative due to addition of bias in last call

    if(elapsed > (signed int)m_interval)    // ugly stuff here since DWORD is unsigned
    {
        DWORD bias = elapsed%m_interval;
        DWORD rest = elapsed - bias;
        int n = rest / m_interval;
        m_lasttick = GetTickCount() + bias;
        return n;
    }
    else
    {
        return 0;
    }

}

