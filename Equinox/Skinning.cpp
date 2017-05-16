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
			bone->Name = string(aiMesh->mBones[i]->mName.C_Str());
			bone->NumWeights = aiMesh->mBones[i]->mNumWeights;
			bone->Weights = new BoneWeight[bone->NumWeights];
			aiMatrix4x4 bind = aiMesh->mBones[i]->mOffsetMatrix;
			bone->Bind = float4x4(	bind.a1, bind.a2, bind.a3, bind.a4, 
									bind.b1, bind.b2, bind.b3, bind.b4, 
									bind.c1, bind.c2, bind.c3, bind.c4, 
									bind.d1, bind.d2, bind.d3, bind.d4);

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

void Skinning::UpdateVertexs(float4x4 globalMatrixBindPos, float4x4 gloabalMatrixPos, float weight)
{



}
