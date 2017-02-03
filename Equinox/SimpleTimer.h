#ifndef __SIMPLETIMER_H__
#define __SIMPLETIMER_H__

#include "SDL/include/SDL.h"

class SimpleTimer
{
public:
	SimpleTimer()
	{
		pausedTicks = 0, runTicks = 0;
		running = false;
	}

	void Start()
	{
		running = true;
		runTicks = SDL_GetTicks();
	}

	unsigned int Stop()
	{
		if(running)
		{
			running = false;
			return pausedTicks = SDL_GetTicks() - runTicks;
		}
		return pausedTicks;
	}

	unsigned int Read() const
	{
		return pausedTicks;
	}

private:
	unsigned int pausedTicks, runTicks;
	bool running;
};


#endif // __SIMPLETIMER_H__