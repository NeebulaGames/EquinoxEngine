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
	double Duration = 0.f;
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


class ModuleAnimation : public Module
{
public:
	typedef unsigned AnimInstanceID;

	ModuleAnimation(bool start_enabled = true);
	~ModuleAnimation();

	bool CleanUp() override;
	update_status Update() override;

	void Load(const char* name, const char* file);

	AnimInstanceID Play(const char* name);
	void Stop(AnimInstanceID id);

	bool GetTransform(AnimInstanceID instance, const char* channelName, aiVector3D& position, aiQuaternion& rotation) const;

	aiVector3D InterpVector3D(const aiVector3D& first, const aiVector3D& second, float lambda) const;
	aiQuaternion InterpQuaternion(const aiQuaternion& first, const aiQuaternion& second, float lambda) const;

private:
	typedef std::map<std::string, Anim*> AnimMap;
	typedef std::vector<AnimInstance*> InstanceList;
	typedef std::vector<AnimInstanceID> HoleList;

	AnimMap _animations;
	InstanceList _instances;
	HoleList _holes;
};

#endif // __MODULEANIMATION_H__