#include <assimp/scene.h>
#include "Skinning.h"
#include "MeshComponent.h"

using namespace std;

Skinning::Skinning()
{
}

Skinning::~Skinning()
{
}

void Skinning::Load(aiMesh* aiMesh , Mesh* mesh)
{
	if (aiMesh->HasBones())
	{
		mesh->Bones.resize(aiMesh->mNumBones);
		for (int i = 0; i < aiMesh->mNumBones; ++i)
		{
			Bone* bone = new Bone();
			bone->NumWeights = aiMesh->mBones[i]->mNumWeights;
			bone->Weights = new BoneWeight[bone->NumWeights];
			for (int j = 0; j < aiMesh->mBones[i]->mNumWeights; ++j)
			{
				bone->Weights[j] = BoneWeight();
				bone->Weights[j].Vertex = aiMesh->mBones[i]->mWeights[j].mVertexId;
				bone->Weights[j].Weight = aiMesh->mBones[i]->mWeights[j].mWeight;
			}
			mesh->Bones[i] = bone;
		}
	}
}
