#include "DebugDrawer.h"
#include <GL/glew.h>


DebugDrawer::DebugDrawer()
{
	mode = DBG_NoDebug;
}


DebugDrawer::~DebugDrawer()
{
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
 	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glColor3f(color.x(), color.y(), color.z());
	glVertex3fv(from.get128().m128_f32);
	glVertex3fv(to.get128().m128_f32);
	glEnd();

	glEnable(GL_LIGHTING);
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
}

void DebugDrawer::setDebugMode(int debugMode)
{
}

int DebugDrawer::getDebugMode() const
{
	return btIDebugDraw::DebugDrawModes::DBG_MAX_DEBUG_DRAW_MODE;
}
