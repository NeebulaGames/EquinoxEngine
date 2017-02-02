#ifndef __CUBE_H__
#define __CUBE_H__

#include "Primitive.h"
class Cube :
	public Primitive
{
public:
	Cube(const fPoint3& position, Quat& rotation, const fPoint3& normals);
	~Cube();


	void Draw() override;
};

#endif // __CUBE_H__
