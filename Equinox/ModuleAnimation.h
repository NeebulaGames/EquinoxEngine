#ifndef __MODULEANIMATION_H__
#define __MODULEANIMATION_H__

#include "Module.h"
#include <map>
#include <MathGeoLib/include/Math/float3.h>

struct Channel
{
	std::string NodeName;
	std::vector<float3> Positions;
	std::vector<Quat> Rotations;
};

struct Animation
{
	double Duration = 0.f;
	std::vector<Channel> Channels;
};

class ModuleAnimation : public Module
{
public:
	ModuleAnimation(bool start_enabled = true);
	~ModuleAnimation();

	bool CleanUp() override;

	void Load(const char* name, const char* file);

private:
	typedef std::map<std::string, Animation> AnimationsMap;

	AnimationsMap _animations;
};

#endif // __MODULEANIMATION_H__