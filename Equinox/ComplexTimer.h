#pragma once
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
		ticks = SDL_GetPerformanceCounter();
		started = true;
	}

	Uint64 Read() const
	{
		return started ? (ticks - SDL_GetPerformanceCounter()) / PerformanceFrequency : ticks;
	}

	Uint64 Stop()
	{
		ticks = (SDL_GetPerformanceCounter() - ticks) / PerformanceFrequency;
		started = false;
		return ticks;
	}

private:
	bool started;
	Uint64 ticks;
};
