#include "Cube.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3.h>
#include <MathGeoLib/include/Math/MathFunc.h>


Cube::Cube() : 
	Primitive() {}

Cube::Cube(const float3& position, const Quat& rotation) :
	Primitive(position, rotation) {}

Cube::Cube(const float3& position, const Quat& rotation, int textureId) : Primitive(position, rotation), _textureId(textureId)
{
}

Cube::Cube(const float3& position, const Quat& rotation, const float3& color) :
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
	//a,b,d
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1, 0, 0);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1, 1, 0);

	//a,d,c
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1, 1, 0);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0, 1, 0);

	//c,f,h
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1, 0, 0);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1, 0, 1);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1, 1, 1);

	//b,h,d
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1, 0, 0);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1, 1, 1);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1, 1, 0);

	//f,e,g
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1, 0, 1);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0, 1, 1);

	//f,g,h
	glTexCoord2f(0.f, 0.f);
	glVertex3f(1, 0, 1);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0, 1, 1);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1, 1, 1);

	//e,a,c
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0, 1, 0);

	//e,b,g
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0, 1, 0);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0, 1, 1);

	//b,a,e
	glTexCoord2f(0.f, 1.f);
	glVertex3f(1, 0, 0);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0, 0, 1);

	//c,e,f
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1, 0, 0);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1, 0, 1);

	//d,h,g
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1, 1, 0);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1, 1, 1);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0, 1, 1);

	//d,g,b
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1, 1, 0);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0, 1, 1);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0, 1, 0);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}
