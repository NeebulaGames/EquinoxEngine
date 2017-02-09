#ifndef __CUBE_H__
#define __CUBE_H__

#include "Primitive.h"

class Cube :
	public Primitive
{
public:
	Cube();
	Cube(const float3& position, Quat& rotation);
	Cube(const float3& position, Quat& rotation, const float3& color);
	~Cube();

	void Draw() override;
};

#endif // __CUBE_H__
