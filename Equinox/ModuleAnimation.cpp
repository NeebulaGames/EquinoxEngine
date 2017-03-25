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
			RELEASE(channel);

		RELEASE(element.second);
	}

	_animations.clear();

	return true;
}

update_status ModuleAnimation::Update()
{
	for (AnimInstance* animInstance : _instances)
		if(animInstance != nullptr)
			animInstance->time += App->DeltaTime;
}

void ModuleAnimation::Load(const char* name, const char* file)
{
	LOG("Loading animation %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s", file);

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	aiAnimation** animations = scene->mAnimations;

	Anim* anim = new Anim();
	anim->Duration = animations[0]->mDuration;
	anim->Channels = std::vector<NodeAnim*>(animations[0]->mNumChannels);

	for (unsigned int i = 0; i < animations[0]->mNumChannels; ++i)
	{
		aiNodeAnim* aiNodeAnim = animations[0]->mChannels[i];
		
		anim->Channels[i] = new NodeAnim();
		anim->Channels[i]->NodeName = aiNodeAnim->mNodeName.C_Str();

		for(unsigned int j = 0; j < aiNodeAnim->mNumPositionKeys; ++j)
		{
			aiVector3D position = aiNodeAnim->mPositionKeys[j].mValue;
			anim->Channels[i]->Positions.push_back(&float3(position.x, position.y, position.z));
		}

		for (unsigned int j = 0; j < aiNodeAnim->mNumRotationKeys; ++j)
		{
			aiQuaternion rotation = aiNodeAnim->mRotationKeys[j].mValue;
			anim->Channels[i]->Rotations.push_back(&Quat(rotation.x, rotation.y, rotation.z, rotation.w));
		}
	}

	_animations[name] = anim;
	aiReleaseImport(scene);
}

ModuleAnimation::AnimInstanceID ModuleAnimation::Play(const char* name)
{
	AnimInstance* animInstance = new AnimInstance();
	animInstance->anim = _animations[name];
	unsigned pos;

	if(_holes.empty())
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

	return pos;
}


void ModuleAnimation::Stop(AnimInstanceID id)
{
	_holes.push_back(id);
	RELEASE(_instances[id]);
}

bool ModuleAnimation::GetTransform(AnimInstanceID instance, const char* channelName, aiVector3D& position, aiQuaternion& rotation) const
{
	for (NodeAnim* channel : _instances[instance]->anim->Channels)
	{
		if (channel->NodeName == channelName)
			channel->Positions;//Continue
	}
}
