#define _USE_MATH_DEFINES
#include <cmath> 
#include "Cylinder.h"
#include <MathGeoLib/include/Math/MathFunc.h>


::Cylinder::Cylinder(const fPoint3& position, Quat& rotation, const fPoint3& normals, GLfloat radius, GLfloat height) :
	Primitive(position, rotation, normals), Radius(radius), Height(height)
{
}


::Cylinder::~Cylinder()
{
}

void ::Cylinder::Draw()
{
	glPushMatrix();
	glTranslatef(Position.x, Position.y, Position.z);
	float3 axis = Rotation.Axis();
	glRotatef(RadToDeg(Rotation.Angle()), axis.x, axis.y, axis.z);

	glColor3f(0, 0, 25.f); //TODO:Remove, take by param

	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat angle = 0.0;
	GLfloat angle_stepsize = 0.1;

	/** Draw the tube */
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * M_PI) {
		x = Radius * cos(angle);
		y = Radius * sin(angle);
		glVertex3f(x, y, Height);
		glVertex3f(x, y, 0.0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(Radius, 0.0, Height);
	glVertex3f(Radius, 0.0, 0.0);
	glEnd();

	/** Draw the circle on top of cylinder */
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * M_PI) {
		x = Radius * cos(angle);
		y = Radius * sin(angle);
		glVertex3f(x, y, Height);
		angle = angle + angle_stepsize;
	}
	glVertex3f(Radius, 0.0, Height);
	glEnd();

	glPopMatrix();
}