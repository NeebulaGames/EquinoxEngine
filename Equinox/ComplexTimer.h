#ifndef __COMPLEX_TIMER_H__
#define __COMPLEX_TIMER_H__
#include <SDL/include/SDL.h>

const Uint64 PerformanceFrequency = SDL_GetPerformanceFrequency();

class ComplexTimer
{
public:
	ComplexTimer() : started(false), ticks(0)
	{
	}

	void Start()
	{
		ticks = SDL_GetPerformanceCounter() * 1E6;
		started = true;
	}

	double Read() const
	{
		return started ? (double)(((SDL_GetPerformanceCounter() * 1E6) - ticks) / PerformanceFrequency) : count;
	}

	double Stop()
	{
		count = Read();
		started = false;
		return count;
	}

private:
	bool started;
	double ticks;
	double count;
};

#endif
