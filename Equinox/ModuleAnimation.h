#ifndef __MODULEANIMATION_H__
#define __MODULEANIMATION_H__

#include "Module.h"
#include <map>
#include <MathGeoLib/include/Math/float3.h>

struct NodeAnim
{
	std::string NodeName;
	std::vector<float3*> Positions;
	std::vector<Quat*> Rotations;
};

struct Anim
{
	unsigned Duration = 0;
	std::vector<NodeAnim*> Channels;
};

struct AnimInstance
{
	Anim* anim;
	unsigned time = 0;
	bool loop = true;
	
	AnimInstance* next = nullptr;
	unsigned blend_duration = 0;
	unsigned blend_time = 0;
};

typedef unsigned AnimInstanceID;

class ModuleAnimation : public Module
{
public:
	ModuleAnimation(bool start_enabled = true);
	~ModuleAnimation();

	bool CleanUp() override;
	update_status Update() override;

	void Load(const char* name, const char* file);

	AnimInstanceID Play(const char* name);
	void Stop(AnimInstanceID id);

	bool GetTransform(AnimInstanceID id, const char* channelName, float3& position, Quat& rotation) const;

private:
	static float3 InterpVector3D(const float3& first, const float3& second, float lambda);
	static Quat InterpQuaternion(const Quat& first, const Quat& second, float lambda);

	typedef std::map<std::string, Anim*> AnimMap;
	typedef std::vector<AnimInstance*> InstanceList;
	typedef std::vector<AnimInstanceID> HoleList;

	AnimMap _animations;
	InstanceList _instances;
	HoleList _holes;
};

#endif // __MODULEANIMATION_H__