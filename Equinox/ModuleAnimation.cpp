#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "ModuleAnimation.h"
#include <MathGeoLib/include/Math/Quat.h>

ModuleAnimation::ModuleAnimation(bool start_enabled) : Module(start_enabled)
{
}

ModuleAnimation::~ModuleAnimation()
{
}

bool ModuleAnimation::CleanUp()
{
	AnimationsMap::iterator it = _animations.begin();
	for (std::pair<std::string, Animation*> element : _animations)
	{
		for (Channel* channel : element.second->Channels)
		{
			RELEASE(channel);
		}
		RELEASE(element.second);
	}

	_animations.clear();

	return true;
}

void ModuleAnimation::Load(const char* name, const char* file)
{
	LOG("Loading animation %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s", file);

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	aiAnimation** animations = scene->mAnimations;

	Animation* anim = new Animation();
	anim->Duration = animations[0]->mDuration;
	anim->Channels = std::vector<Channel*>(animations[0]->mNumChannels);

	for (unsigned int i = 0; i < animations[0]->mNumChannels; ++i)
	{
		aiNodeAnim* aiNodeAnim = animations[0]->mChannels[i];
		
		anim->Channels[i] = new Channel();
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
