#ifndef __PLANE_H__
#define __PLANE_H__

#include "Primitive.h"

class Plane : 
	public Primitive
{
public:
	Plane(float planeSize);
	Plane(const float3& position, const Quat& rotation, float planeSize);
	Plane(const float3& position, const Quat& rotation, const float3& color, float planeSize);
	~Plane();

	void Draw() override;

	float PlaneSize;
};

#endif // __PLANE_H__

