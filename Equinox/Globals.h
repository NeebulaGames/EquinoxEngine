#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "MemLeaks.h"
#include "SDL/include/SDL_rect.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Geometry/AABB.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__)

void log(const char file[], int line, const char* format, ...);

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define CLAMP( v, min, max ) ( MAX(min, MIN(v, max)) )
#define DEG2RAD(deg) (deg * float(M_PI) / 180.0f)
#define ZTOY(z, angle) ( cos(DEG2RAD(angle)) * z )

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Useful typedefs ---------
typedef unsigned int uint;

// Deletes a buffer
#define RELEASE( x ) \
	{									  \
	   if( x != nullptr )   \
	   {						      \
		 delete x;                  \
		 x = nullptr;             \
	   }                      \
	}

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{                              \
	   if( x != nullptr )              \
	   {                            \
		   delete[] x;                \
		   x = nullptr;                    \
		 }                            \
							  \
	 }

// Configuration -----------
#define SCREEN_SIZE 1
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define FULLSCREEN false
#define VSYNC true
#define RESIZABLE true
#define TITLE "Equinox Engine Editor"

inline void DrawBoundingBox(AABB& boundingBox)
{
	glPushMatrix();

	GLboolean light = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	if (boundingBox.IsFinite())
	{
		glLineWidth(3.f);
		glBegin(GL_LINES);
		glColor3f(0.f, 1.f, 0.f);
		vec points[8];
		boundingBox.GetCornerPoints(points);

		// LEFT SIDE
		glVertex3fv(&points[0][0]);
		glVertex3fv(&points[1][0]);

		glVertex3fv(&points[0][0]);
		glVertex3fv(&points[2][0]);

		glVertex3fv(&points[2][0]);
		glVertex3fv(&points[3][0]);

		glVertex3fv(&points[3][0]);
		glVertex3fv(&points[1][0]);

		// BACK SIDE
		glVertex3fv(&points[0][0]);
		glVertex3fv(&points[4][0]);

		glVertex3fv(&points[2][0]);
		glVertex3fv(&points[6][0]);

		glVertex3fv(&points[4][0]);
		glVertex3fv(&points[6][0]);

		// RIGHT SIDE
		glVertex3fv(&points[6][0]);
		glVertex3fv(&points[7][0]);

		glVertex3fv(&points[4][0]);
		glVertex3fv(&points[5][0]);

		glVertex3fv(&points[7][0]);
		glVertex3fv(&points[5][0]);

		// FRONT SIDE
		glVertex3fv(&points[1][0]);
		glVertex3fv(&points[5][0]);

		glVertex3fv(&points[3][0]);
		glVertex3fv(&points[7][0]);

		glEnd();
	}

	if (light)
		glEnable(GL_LIGHTING);

	glPopMatrix();
}

#endif //__GLOBALS_H__