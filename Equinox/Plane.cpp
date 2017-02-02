#include "Plane.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3.h>
#include <MathGeoLib/include/Math/MathFunc.h>
#include "Globals.h"

::Plane::Plane(const fPoint3& position, Quat& rotation, const fPoint3& normals, int plane_size) :
	Primitive(position, rotation, normals), PlaneSize(plane_size) {};

void ::Plane::Draw()
{
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
	float3 axis = Rotation.Axis();
	glRotatef(RadToDeg(Rotation.Angle()), axis.x, axis.y, axis.z);

	glColor3f(25.f, 0, 0); //TODO:Remove, take by param

	glBegin(GL_QUADS);
	glVertex3f(-PlaneSize, -0.001, -PlaneSize);
	glVertex3f(-PlaneSize, -0.001, PlaneSize);
	glVertex3f(PlaneSize, -0.001, PlaneSize);
	glVertex3f(PlaneSize, -0.001, -PlaneSize);
	glEnd();

	glBegin(GL_LINES);
	for (int i = -PlaneSize; i <= PlaneSize; i++) {
		if (i == -PlaneSize) { glColor3f(.6, .3, .3); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(i, 0, -PlaneSize);
		glVertex3f(i, 0, PlaneSize);
		if (i == -PlaneSize) { glColor3f(.3, .3, .6); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(-PlaneSize, 0, i);
		glVertex3f(PlaneSize, 0, i);
	};
	glEnd();
	glPopMatrix();
}
