#ifndef __PLANE_H__
#define __PLANE_H__
#include "Primitive.h"

class Plane : public Primitive
{
public:
	Plane(const fPoint3& position, Quat& rotation, const fPoint3& normals, int plane_size);
	~Plane() {};

	void Draw() override;

	int PlaneSize;
};

#endif // __PLANE_H__

