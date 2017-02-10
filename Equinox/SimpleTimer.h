#ifndef __SIMPLETIMER_H__
#define __SIMPLETIMER_H__

#include "SDL/include/SDL.h"

class SimpleTimer
{
public:
	SimpleTimer() : started(false), ticks(0)
	{
	}

	void Start()
	{
		ticks = SDL_GetTicks();
		started = true;
	}

	unsigned int Read() const
	{
		return started ? SDL_GetTicks() - ticks : ticks;
	}

	unsigned int Stop()
	{
		ticks = Read();
		started = false;
		return ticks;
	}

private:
	bool started;
	unsigned int ticks;
};

#endif // __SIMPLETIMER_H__