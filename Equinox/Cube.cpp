#include "Cube.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3.h>


Cube::Cube(const fPoint3& position, Quat& rotation, const fPoint3& normals) : Primitive(position, rotation, normals)
{
}


Cube::~Cube()
{
}

void Cube::Draw()
{
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
	float3 axis = Rotation.Axis();
 	glRotatef(Rotation.Angle(), axis.x, axis.y, axis.z);

	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);

	//a,d,c
	glVertex3f(0, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(0, 1, 0);

	//b,f,h
	glVertex3f(1, 0, 0);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);

	//b,h,d
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, 0);

	//f,e,g
	glVertex3f(1, 0, 1);
	glVertex3f(0, 0, 1);
	glVertex3f(0, 1, 1);

	//f,g,h
	glVertex3f(1, 0, 1);
	glVertex3f(0, 1, 1);
	glVertex3f(1, 1, 1);

	//e,a,c
	glVertex3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);

	//e,c,g
	glVertex3f(0, 0, 1);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 1, 1);

	//b,a,e
	glVertex3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	//b,e,f
	glVertex3f(1, 0, 0);
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);

	//d,h,g
	glVertex3f(1, 1, 0);
	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);

	//d,g,c
	glVertex3f(1, 1, 0);
	glVertex3f(0, 1, 1);
	glVertex3f(0, 1, 0);

	glEnd();

	glPopMatrix();
}
