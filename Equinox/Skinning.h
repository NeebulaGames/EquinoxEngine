#ifndef __SKINNING_H__
#define __SKINNING_H__

#include <MathGeoLib/include/Math/float4x4.h>

struct BoneWeight
{
	unsigned Vertex = 0;
	float Weight = 0.0f;
};

struct Bone
{
	std::string Name;
	BoneWeight* weights = nullptr;
	unsigned NumWeights = 0;
	float4x4 Bind;
};

class Skinning
{
public:
	Skinning();
	~Skinning();



};

#endif // __SKINNING_H__