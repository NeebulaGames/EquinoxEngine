#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Primitive.h"
#include <GL/glew.h>
#include <vector>

class Sphere : public Primitive
{
public:
	Sphere(const fPoint3& position, Quat& rotation, const fPoint3& normals, float radius, unsigned int rings, unsigned int sectors);
	~Sphere();

	void Draw() override;

protected:
	std::vector<GLfloat> verticesVector;
	std::vector<GLfloat> normalsVector;
	std::vector<GLfloat> texcoordsVector;
	std::vector<GLushort> indices;
};

#endif // __Sphere_H__
