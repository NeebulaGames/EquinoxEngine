#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "ModuleAnimation.h"
#include <MathGeoLib/include/Math/Quat.h>
#include "Engine.h"

ModuleAnimation::ModuleAnimation(bool start_enabled) : Module(start_enabled)
{
}

ModuleAnimation::~ModuleAnimation()
{
}

bool ModuleAnimation::CleanUp()
{
	AnimMap::iterator it = _animations.begin();
	for (std::pair<std::string, Anim*> element : _animations)
	{
		for (NodeAnim* channel : element.second->Channels)
		{
			for(float3* position : channel->Positions)
				RELEASE(position);

			for(Quat* rotation : channel->Rotations)
				RELEASE(rotation);

			RELEASE(channel);
		}

		RELEASE(element.second);
	}

	for (AnimInstance* animInstance : _instances)
	{
		RELEASE(animInstance->next);
		RELEASE(animInstance);
	}
		

	_animations.clear();

	free(_animationNames);

	return true;
}

update_status ModuleAnimation::Update(float DeltaTime)
{
	for (AnimInstance* animInstance : _instances)
	{
		if (animInstance != nullptr)
		{
			animInstance->time = 
				static_cast<unsigned>(
					fmod(animInstance->time + App->DeltaTime * 1E3, float(animInstance->anim->Duration))
				);

			if(animInstance->next)
			{
				AnimInstance* nextAnimInstance = animInstance->next;
				nextAnimInstance->time = 
					static_cast<unsigned>(fmod(nextAnimInstance->time + App->DeltaTime * 1E3, float(nextAnimInstance->anim->Duration)));
				animInstance->blend_time += static_cast<unsigned>(App->DeltaTime * 1E3);
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleAnimation::Load(const char* name, const char* file)
{
	LOG("Loading animation %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s", file);

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	aiAnimation** animations = scene->mAnimations;

	Anim* anim = new Anim();
	anim->Duration = static_cast<unsigned>(animations[0]->mDuration / animations[0]->mTicksPerSecond * 1E3);
	anim->Channels = std::vector<NodeAnim*>(animations[0]->mNumChannels);

	for (unsigned int i = 0; i < animations[0]->mNumChannels; ++i)
	{
		aiNodeAnim* aiNodeAnim = animations[0]->mChannels[i];
		
		anim->Channels[i] = new NodeAnim();
		anim->Channels[i]->NodeName = aiNodeAnim->mNodeName.C_Str();

		for(unsigned int j = 0; j < aiNodeAnim->mNumPositionKeys; ++j)
		{
			aiVector3D position = aiNodeAnim->mPositionKeys[j].mValue;
			anim->Channels[i]->Positions.push_back(new float3(position.x, position.y, position.z));
		}

		for (unsigned int j = 0; j < aiNodeAnim->mNumRotationKeys; ++j)
		{
			aiQuaternion rotation = aiNodeAnim->mRotationKeys[j].mValue;
			anim->Channels[i]->Rotations.push_back(new Quat(rotation.x, rotation.y, rotation.z, rotation.w));
		}
	}

	_animations[name] = anim;
	aiReleaseImport(scene);
}

AnimInstanceID ModuleAnimation::Play(const char* name)
{
	int pos = -1;
	if (_animations.find(name) != _animations.end())
	{
		AnimInstance* animInstance = new AnimInstance();
		animInstance->anim = _animations[name];
		if (_holes.empty())
		{
			_instances.push_back(animInstance);
			pos = _instances.size() - 1;
		}
		else
		{
			pos = _holes.at(_holes.size() - 1);
			_instances.at(pos) = animInstance;
			_holes.pop_back();
		}
	}
	return pos;
}


void ModuleAnimation::Stop(AnimInstanceID id)
{
	if(id >= 0)
	{
		_holes.push_back(id);
		RELEASE(_instances[id]);
	}
}

void ModuleAnimation::BlendTo(AnimInstanceID id, const char* name, unsigned blend_time)
{
	if (!(_animations.find(name) != _animations.end()))
		return Stop(id);

	AnimInstance* animInstance = new AnimInstance();
	animInstance->anim = _animations[name];
	
	_instances[id]->next = animInstance;
	_instances[id]->blend_duration = blend_time;
}

bool ModuleAnimation::GetTransform(AnimInstanceID id, const char* channelName, float3& position, Quat& rotation) 
{
	float lambda = float(_instances[id]->blend_time) / _instances[id]->blend_duration;
	AnimInstance* nextAnimInstance = _instances[id]->next;

	if(lambda > 1)
	{
		RELEASE(_instances[id]);
		_instances[id] = nextAnimInstance;
	}

	UpdateTransform(_instances[id], channelName, position, rotation);

	if (_instances[id]->next && _instances[id]->blend_duration != 0)
	{
		float3 positionNext;
		Quat rotationNext;
		
		UpdateTransform(nextAnimInstance, channelName, positionNext, rotationNext);		
	
		position = float3::Lerp(position, positionNext, lambda);
		rotation = InterpQuaternion(rotation, rotationNext, lambda);
	}

	return true;
}

bool ModuleAnimation::UpdateTransform(AnimInstance* instance, const char* channelName, float3& position, Quat& rotation) const
{
	Anim* animation = instance->anim;
	NodeAnim* node = nullptr;
	for (NodeAnim* channel : instance->anim->Channels)
	{
		if (channel->NodeName == channelName)
		{
			node = channel;
			break;
		}
	}

	if (!node)
		return false;

	float posKey = float(instance->time * (node->Positions.size() - 1)) / float(animation->Duration);
	float rotKey = float(instance->time * (node->Rotations.size() - 1)) / float(animation->Duration);

	unsigned posIndex = unsigned(posKey);
	unsigned rotIndex = unsigned(rotKey);

	float posLambda = posKey - float(posIndex);
	float rotLambda = rotKey - float(rotIndex);

	if (node->Positions.size() > 1)
		position = float3::Lerp(*node->Positions[posIndex], *node->Positions[posIndex + 1], posLambda);
	else
		position = *node->Positions[posIndex];

	if (node->Rotations.size() > 1)
		rotation = InterpQuaternion(*node->Rotations[rotIndex], *node->Rotations[rotIndex + 1], rotLambda);
	else
		rotation = *node->Rotations[rotIndex];

	return true;
}

char* ModuleAnimation::GetAnimationLabels()
{
	char combo[256] = { "None" };
	size_t totalSize = 5;

	for (AnimMap::iterator it = _animations.begin(); it != _animations.end(); ++it)
	{
		std::string animName(it->first, '\0');
		size_t lenght = animName.size();
		memcpy(combo + totalSize, animName.c_str(), totalSize + lenght + 1);
		totalSize += lenght + 1;
	}

	free(_animationNames);
	_animationNames = static_cast<char*>(malloc(totalSize + 1));
	_animationNames[totalSize] = '\0';
	memcpy(_animationNames, combo, totalSize);

	return _animationNames;
}

int ModuleAnimation::GetLabelByInstance(AnimInstanceID animInstance)
{
	if (animInstance == -1)
		return 0;

	int i = 1;
	for (AnimMap::iterator it = _animations.begin(); it != _animations.end(); ++it)
	{
		if (_instances[animInstance]->anim == static_cast<Anim*>(it->second))
			return i;
		++i;
	}
	return 0;
}

std::string ModuleAnimation::GetNameAnimByLabel(int label)
{
	int i = 1;
	std::string none = std::string("None");

	if (label == 0)
		return none;
	
	for (AnimMap::iterator it = _animations.begin(); it != _animations.end(); ++it)
	{
		if (i == label)
			return static_cast<std::string>(it->first);
		++i;
	}

	return none;
}

Anim* ModuleAnimation::GetAnimByName(std::string name)
{
	if (_animations.find(name) == _animations.end())
		return nullptr;

	return _animations[name];
}

bool ModuleAnimation::isAnimInstanceOfAnim(AnimInstanceID animInstance, Anim* anim)
{
	if (animInstance < 0)
		return anim == nullptr;
	
	return _instances[animInstance]->anim == anim;
}

Quat ModuleAnimation::InterpQuaternion(const Quat& first, const Quat& second, float lambda)
{
	aiQuaternion result;

	float dot = first.x*second.x + first.y*second.y + first.z*second.z + first.w*second.w;

	if(dot >= 0.0f)
	{
		result.x = first.x*(1.0f - lambda) + second.x*lambda;
		result.y = first.y*(1.0f - lambda) + second.y*lambda;
		result.z = first.z*(1.0f - lambda) + second.z*lambda;
		result.w = first.w*(1.0f - lambda) + second.w*lambda;
	}
	else
	{
		result.x = first.x*(1.0f - lambda) + second.x*-lambda;
		result.y = first.y*(1.0f - lambda) + second.y*-lambda;
		result.z = first.z*(1.0f - lambda) + second.z*-lambda;
		result.w = first.w*(1.0f - lambda) + second.w*-lambda;
	}

	result.Normalize();

	return Quat(result.x, result.y, result.z, result.w);
}
