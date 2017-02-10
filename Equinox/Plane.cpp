#include "Plane.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3.h>
#include <MathGeoLib/include/Math/MathFunc.h>
#include "Globals.h"


::Plane::Plane(int planeSize) : 
	Primitive(), PlaneSize(planeSize) {}

::Plane::Plane(const float3& position, Quat& rotation, int planeSize) :
	Primitive(position, rotation), PlaneSize(planeSize) {}

::Plane::Plane(const float3& position, Quat& rotation, const float3& color, int planeSize) : 
	Primitive(position, rotation, color), PlaneSize(planeSize) {}

::Plane::~Plane() {}

void ::Plane::Draw()
{
	glDepthMask(GL_FALSE);
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
	float3 axis = Rotation.Axis();
	glRotatef(RadToDeg(Rotation.Angle()), axis.x, axis.y, axis.z);

	glColor4f(Color.x, Color.y, Color.z, 0.f);

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
	glDepthMask(GL_TRUE);
}
