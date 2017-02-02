#ifndef __PLANE__
#define __PLANE__
#include "Primitive.h"

class Plane : public Primitive
{
public:
	Plane(const fPoint3& position, Quat& rotation, const fPoint3& normals);
	~Plane() {};

	void Draw() override;

	int Height, Width;
};

#endif

