#include "Skinning.h"
#include <assimp/scene.h>

Skinning::Skinning()
{
}

Skinning::~Skinning()
{
}

void Skinning::Load(const aiScene* scene)
{
	if (scene->mMeshes[0]->HasBones())
	{
		
	}
}
