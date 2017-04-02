#pragma once
#include "Module.h"
#include "SimpleTimer.h"
#include "ComplexTimer.h"

class ModuleStats :
	public Module
{
	friend class Engine;
public:

	float CurrentFPS() const { return _current_fps; }
	float CurrentAvgFPS() const { return _current_avg; }
	float PreciseUptime() const { return _total_complex_time.Read(); }
	int Uptime() const { return _total_simple_time.Read(); }
	float DeltaTime() const { return _delta_time; }
	float FrameCount() const { return _total_frames; }

private:
	float _total_frames = 0.f;
	float _delta_time = 0.f;
	float _current_avg = 0;
	float _current_fps = 0.f;
	ComplexTimer _total_complex_time;
	SimpleTimer _total_simple_time;
};

