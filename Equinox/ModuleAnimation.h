#ifndef __MODULEANIMATION_H__
#define __MODULEANIMATION_H__

#include "Module.h"
#include <map>
#include <MathGeoLib/include/Math/float3.h>

struct Channel
{
	const char* NodeName;
	float3* Positions;
	Quat* Rotations;
};

struct Animation
{
	double Duration = 0.f;
	Channel* Channels;
};

class ModuleAnimation : public Module
{
public:
	ModuleAnimation(bool start_enabled = true);
	~ModuleAnimation();

	void Load(const char* name, const char* file);

private:
	typedef std::map<const char*, Animation> AnimationsMap;

	AnimationsMap _animations;
};

#endif // __MODULEANIMATION_H__