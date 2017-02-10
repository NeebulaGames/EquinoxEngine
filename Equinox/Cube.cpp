#include "Cube.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3.h>
#include <MathGeoLib/include/Math/MathFunc.h>


Cube::Cube() : 
	Primitive() {}

Cube::Cube(const float3& position, Quat& rotation) : 
	Primitive(position, rotation) {}

Cube::Cube(const float3& position, Quat& rotation, int textureId) : Primitive(position, rotation), _textureId(textureId)
{
}

Cube::Cube(const float3& position, Quat& rotation, const float3& color) : 
	Primitive(position, rotation, color) {}

Cube::~Cube() {}

void Cube::Draw()
{
	glPushMatrix();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTranslatef(Position.x, Position.y, Position.z);
	float3 axis = Rotation.Axis();
	glRotatef(RadToDeg(Rotation.Angle()), axis.x, axis.y, axis.z);

	glColor3f(Color.x, Color.y, Color.z);

	glBegin(GL_TRIANGLES);

	// a, b, c
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, 0);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);

	// c, d, a
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);

	// a, d, e
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);

	// e, f, a
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 1.f, 0.f);

	// a, f, g
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);

	// g, b, a
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);

	// g, f, e
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);

	// e, h, g
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, -1.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);

	// g, h, c
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, -1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);

	// c, b, g
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 1.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 1.f, -1.f);

	// h, e, d
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, -1.f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1.f, 0.f, -1.f);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);

	// d, c, h
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, -1.f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}
