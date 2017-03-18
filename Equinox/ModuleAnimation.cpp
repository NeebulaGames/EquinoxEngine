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

void ModuleAnimation::Load(const char* name, const char* file)
{
	LOG("Loading animation %s", file);
	char filePath[256];
	sprintf_s(filePath, "%s", file);

	const aiScene* scene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);

	aiAnimation** animations = scene->mAnimations;

	Animation anim;

	anim.Duration = animations[0]->mDuration;
	aiNodeAnim** channels = animations[0]->mChannels;


	for (unsigned int i = 0; i < animations[0]->mNumChannels; ++i)
	{
		aiNodeAnim* aiNodeAnim = animations[0]->mChannels[i];
		anim.Channels[i].NodeName = aiNodeAnim->mNodeName.C_Str();
		
		for(unsigned int j = 0; j < aiNodeAnim->mNumPositionKeys; ++j)
		{
			aiVector3D position = aiNodeAnim->mPositionKeys[j].mValue;
			aiQuaternion rotation = aiNodeAnim->mRotationKeys[j].mValue;
			anim.Channels[i].Positions[j] = float3(position.x, position.y, position.z);
			anim.Channels[i].Rotations[j] = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
		}
	}
	_animations[name] = anim;
}
