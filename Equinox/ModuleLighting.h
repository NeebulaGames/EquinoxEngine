#ifndef __MODULELIGHTING_H__
#define __MODULELIGHTING_H__

#include "Module.h"
#include "GL/glew.h"
#include <gl/GL.h>

enum LightType
{
	L_COMBINED,
	L_POINT,
	L_DIRECTIONAL,
	L_SPOTLIGHT,
	L_AMBIENT
};

struct Light
{
public:
	
	GLfloat Ambient[4] = { 0.f, 0.f, 0.f, 1.f };
	GLfloat Diffuse[4] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat Specular[4] = { 1.f, 1.f, 1.f, 1.f };

	bool IsEnabled = false;

	int Number = GL_LIGHT0;
	LightType Type = L_COMBINED;

	// w=1 => position || w=0 => direction (Direct light = 0.f, 0.f, 1.f, 0.f | position light = 0.f, 0.f, 0.f, 1.f)
	GLfloat Position[4] = { 0.f, 0.f, 0.f, 1.f };

	//Required for Spotlight
	GLfloat CutOff = 45.0f;
	GLfloat Direction[3] = { 0.f, 0.f, 0.f };

};

class ModuleLighting : public Module
{
public:
	ModuleLighting();
	~ModuleLighting();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	Light Lights[8];
	Light Ambient;
};

#endif
