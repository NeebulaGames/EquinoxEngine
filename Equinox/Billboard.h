#pragma once
#include <MathGeoLib/include/Math/float3.h>

class Billboard
{
public:
	Billboard();
	~Billboard();

	void SetTexture(unsigned textureId);
	void Draw(const float3& position, const float3& up, const float3& right);
	void GetSize(int& width, int& height);

private:
	unsigned _texture;
	float _width, _height;
};

