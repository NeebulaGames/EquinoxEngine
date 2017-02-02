#include "Plane.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3.h>
#include <MathGeoLib/include/Math/MathFunc.h>
#include "Globals.h"

::Plane::Plane(const fPoint3& position, Quat& rotation, const fPoint3& normals) :
	Primitive(position, rotation, normals) {};

void ::Plane::Draw()
{
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
	float3 axis = Rotation.Axis();
	glRotatef(RadToDeg(Rotation.Angle()), axis.x, axis.y, axis.z);

	glColor3f(1.f, 0, 0); //TODO:Remove, take by param

	glBegin(GL_QUADS);
	glVertex3f(-5, -0.001, -5);
	glVertex3f(-5, -0.001, 5);
	glVertex3f(5, -0.001, 5);
	glVertex3f(5, -0.001, -5);
	glEnd();

	glBegin(GL_LINES);
	for (int i = -5; i <= 5; i++) {
		if (i == -5) { glColor3f(.6, .3, .3); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(i, 0, -5);
		glVertex3f(i, 0, 5);
		if (i == -5) { glColor3f(.3, .3, .6); }
		else { glColor3f(.25, .25, .25); };
		glVertex3f(-5, 0, i);
		glVertex3f(5, 0, i);
	};
	glEnd();
	glPopMatrix();
}
