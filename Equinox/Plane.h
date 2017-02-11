#ifndef __PLANE_H__
#define __PLANE_H__

#include "Primitive.h"

class Plane : 
	public Primitive
{
public:
	Plane(int planeSize);
	Plane(const float3& position, const Quat& rotation, int planeSize);
	Plane(const float3& position, const Quat& rotation, const float3& color, int planeSize);
	~Plane();

	void Draw() override;

	int PlaneSize;
};

#endif // __PLANE_H__

