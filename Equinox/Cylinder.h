#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "Primitive.h"
#include <GL/glew.h>

class Cylinder : 
	public Primitive
{
public:
	Cylinder(GLfloat radius, GLfloat height);
	Cylinder(const float3& position, Quat& rotation, GLfloat radius, GLfloat height);
	Cylinder(const float3& position, Quat& rotation, const float3& color, GLfloat radius, GLfloat height);
	~Cylinder();

	void Draw() override;

	GLfloat Radius;
	GLfloat Height;
};

#endif // __CYLINDER_H__