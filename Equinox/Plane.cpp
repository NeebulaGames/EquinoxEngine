#include "Plane.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3.h>
#include <MathGeoLib/include/Math/MathFunc.h>
#include "Globals.h"


::Plane::Plane(float planeSize) :
	Primitive(), PlaneSize(planeSize) {}

::Plane::Plane(const float3& position, const Quat& rotation, float planeSize) :
	Primitive(position, rotation), PlaneSize(planeSize) {}

::Plane::Plane(const float3& position, const Quat& rotation, const float3& color, float planeSize) :
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
	glVertex3f(-PlaneSize, -0.001f, -PlaneSize);
	glVertex3f(-PlaneSize, -0.001f, PlaneSize);
	glVertex3f(PlaneSize, -0.001f, PlaneSize);
	glVertex3f(PlaneSize, -0.001f, -PlaneSize);
	glEnd();

	glBegin(GL_LINES);
	for (int i = int(-PlaneSize); i <= int(PlaneSize); i++) {
		if (i == -PlaneSize) { glColor3f(.6f, .3f, .3f); }
		else { glColor3f(.25f, .25f, .25f); };
		glVertex3f(float(i), 0.f, -PlaneSize);
		glVertex3f(float(i), 0.f, PlaneSize);
		if (i == -PlaneSize) { glColor3f(.3f, .3f, .6f); }
		else { glColor3f(.25f, .25f, .25f); };
		glVertex3f(-PlaneSize, 0.f, float(i));
		glVertex3f(PlaneSize, 0.f, float(i));
	};
	glEnd();
	glPopMatrix();
	glDepthMask(GL_TRUE);
}
