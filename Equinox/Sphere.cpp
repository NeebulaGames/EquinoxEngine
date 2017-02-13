#define _USE_MATH_DEFINES
#include <cmath> 
#include "Sphere.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/MathFunc.h>
#include <SDL/include/SDL_stdinc.h>


::Sphere::Sphere(float radius, unsigned rings, unsigned sectors) : 
	Primitive()
{
	iniRingsAndSectors(radius, rings, sectors);
}

::Sphere::Sphere(const float3& position, const Quat& rotation, float radius, unsigned rings, unsigned sectors) :
	Primitive(position, rotation)
{
	iniRingsAndSectors(radius, rings, sectors);
}

::Sphere::Sphere(const float3& position, const Quat& rotation, const float3& color, float radius, unsigned rings, unsigned sectors) :
	Primitive(position, rotation, color)
{
	iniRingsAndSectors(radius, rings, sectors);
}

::Sphere::~Sphere() {}

void ::Sphere::Draw()
{
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
	float3 axis = Rotation.Axis();
	glRotatef(RadToDeg(Rotation.Angle()), axis.x, axis.y, axis.z);

	glColor3f(Color.x, Color.y, Color.z);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &verticesVector[0]);
	glNormalPointer(GL_FLOAT, 0, &normalsVector[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoordsVector[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);

	glPopMatrix();
}

void ::Sphere::iniRingsAndSectors(float radius, Uint32 rings, Uint32 sectors)
{
	float const R = 1.f / static_cast<float>(rings - 1);
	float const S = 1.f / static_cast<float>(sectors - 1);
	Uint32 r, s;

	verticesVector.resize(rings * sectors * 3);
	normalsVector.resize(rings * sectors * 3);
	texcoordsVector.resize(rings * sectors * 2);
	std::vector<GLfloat>::iterator v = verticesVector.begin();
	std::vector<GLfloat>::iterator n = normalsVector.begin();
	std::vector<GLfloat>::iterator t = texcoordsVector.begin();
	for (r = 0; r < rings; r++) 
		for (s = 0; s < sectors; s++) {
			float y = float(sin(-M_PI_2 + M_PI * r * R));
			float x = float(cos(2 * M_PI * s * S) * sin(M_PI * r * R));
			float z = float(sin(2 * M_PI * s * S) * sin(M_PI * r * R));

			*t++ = s*S;
			*t++ = r*R;

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			*n++ = x;
			*n++ = y;
			*n++ = z;
		}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings - 1; r++) 
		for (s = 0; s < sectors - 1; s++) {
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
	}
}
