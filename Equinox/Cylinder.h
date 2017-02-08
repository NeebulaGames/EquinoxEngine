#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "Primitive.h"
#include <GL/glew.h>

class Cylinder : public Primitive
{
public:
	Cylinder(const fPoint3& position, Quat& rotation, const fPoint3& normals, GLfloat radius, GLfloat height);
	~Cylinder();

	void Draw() override;

	GLfloat Radius;
	GLfloat Height;
};

#endif // __CYLINDER_H__