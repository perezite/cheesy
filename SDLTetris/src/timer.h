

#ifndef TIMER_H
#define TIMER_H


#include "sprite.h"
#include <SDL.h>
#include <vector>
#include <windows.h>

using namespace std;

// **********************
// Class: Timer
// **********************

class Timer
{
public:
    Timer();
    Timer(DWORD interval);
    void Reset();
    int CheckTick();            // Gives number of ticks since last Start(), last C'tor() or last CheckTick()
private:
    DWORD m_lasttick;
    DWORD m_interval;
};


#endif
