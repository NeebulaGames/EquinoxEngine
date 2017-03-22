#include "CoordinateArrows.h"
#include <GL/glew.h>

CoordinateArrows::CoordinateArrows()
{

}

void CoordinateArrows::Draw()
{
	glPushMatrix();
	GLboolean light = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	//Draw Axis
	glLineWidth(4.0);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);  glVertex3fv(Origin);  glVertex3fv(XP);    // X red axis
	glColor3f(0, 1, 0);  glVertex3fv(Origin);  glVertex3fv(YP);    // Y green axis
	glColor3f(0, 0, 1);  glVertex3fv(Origin);  glVertex3fv(ZP);    // z blue axis
	glEnd();

	//Draw ArrowLikeShape

	glLineWidth(4.0);
	glBegin(GL_LINES);

	//X
	glColor3f(1, 0, 0);

	glVertex3fv(XP); glVertex3f(0.95f, 0.1f, 0);
	glVertex3fv(XP); glVertex3f(0.95f, -0.1f, 0);

	//Y
	glColor3f(0, 1, 0);

	glVertex3fv(YP); glVertex3f(0.1f, 0.95f, 0);
	glVertex3fv(YP); glVertex3f(-0.1f, 0.95f, 0);

	//Y
	glColor3f(0, 1, 0);

	glVertex3fv(YP); glVertex3f(0.1f, 0.95f, 0);
	glVertex3fv(YP); glVertex3f(-0.1f, 0.95f, 0);

	//Z
	glColor3f(0, 0, 1);

	glVertex3fv(ZP); glVertex3f(0, 0.1f, 0.95f);
	glVertex3fv(ZP); glVertex3f(0, -0.1f, 0.95f);

	glEnd();

	//Draw Coords Names
	glLineWidth(2.0);
	glBegin(GL_LINES);

	//X
	glColor3f(1, 0, 0);

	glVertex3f(1.2f, 0.05f, 0); glVertex3f(1.25f, -0.05f, 0);

	glVertex3f(1.2f, -0.05f, 0); glVertex3f(1.25f, 0.05f, 0);

	//Y
	glColor3f(0, 1, 0);

	glVertex3f(0, 1.2f, 0); glVertex3f(0, 1.25f, 0);

	glVertex3f(0, 1.25f, 0); glVertex3f(-0.05f, 1.30f, 0);

	glVertex3f(0, 1.25f, 0); glVertex3f(0.05f, 1.30f, 0);

	//Z
	glColor3f(0, 0, 1);

	glVertex3f(0, -0.05f, 1.2f); glVertex3f(0, -0.05f, 1.25f);

	glVertex3f(0, -0.05f, 1.25f); glVertex3f(0, 0.05f, 1.2f);

	glVertex3f(0, 0.05f, 1.2f); glVertex3f(0, 0.05f, 1.25f);

	glEnd();

	glPopMatrix();
	
	if (light)
		glEnable(GL_LIGHTING);
}
