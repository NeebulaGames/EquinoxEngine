#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Primitive.h"
#include <GL/glew.h>
#include <vector>

class Sphere : 
	public Primitive
{
public:
	Sphere(float radius, unsigned int rings, unsigned int sectors);
	Sphere(const float3& position, const Quat& rotation, float radius, unsigned int rings, unsigned int sectors);
	Sphere(const float3& position, const Quat& rotation, const float3& color, float radius, unsigned int rings, unsigned int sectors);
	~Sphere();

	void Draw() override;

private:
	void iniRingsAndSectors(float radius, unsigned int rings, unsigned int sectors);

protected:
	std::vector<GLfloat> verticesVector;
	std::vector<GLfloat> normalsVector;
	std::vector<GLfloat> texcoordsVector;
	std::vector<GLushort> indices;
};

#endif // __Sphere_H__
