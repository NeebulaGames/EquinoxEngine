#ifndef __SKINNING_H__
#define __SKINNING_H__

#include <MathGeoLib/include/Math/float4x4.h>

struct Mesh;
struct aiScene;
struct aiMesh;

struct BoneWeight
{
	unsigned Vertex = 0;
	float Weight = 0.0f;
};

struct Bone
{
	std::string Name;
	BoneWeight* Weights = nullptr;
	unsigned NumWeights = 0;
	float4x4 Bind;
};

class Skinning
{
public:
	Skinning();
	~Skinning();

	static void Load(aiMesh* aiMesh, Mesh* mesh);
	void UpdateVertexs(Mesh* mesh);


};

#endif // __SKINNING_H__