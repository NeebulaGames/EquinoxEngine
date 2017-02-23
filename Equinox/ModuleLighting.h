#ifndef __MODULELIGHTING_H__
#define __MODULELIGHTING_H__

#include "Module.h"
#include "GL/glew.h"
#include <gl/GL.h>

enum LightType
{
	L_POINT,
	L_DIRECTIONAL,
	L_SPOTLIGHT,
	L_DEFAULT
};

struct Light
{

	GLfloat* Ambient = new GLfloat[4]{ 0.f, 0.f, 0.f, 1.f };
	GLfloat* Diffuse = new GLfloat[4]{ 1.f, 1.f, 1.f, 1.f };
	GLfloat* Specular = new GLfloat[4]{ 1.f, 1.f, 1.f, 1.f };

	bool IsEnabled = false;

	int Number = GL_LIGHT0;
	LightType Type = L_DEFAULT;

	// w=1 => position || w=0 => direction (Direct light = 0.f, 0.f, 1.f, 0.f | position light = 0.f, 0.f, 0.f, 1.f)
	GLfloat* Position = new GLfloat[4]{ 0.f, 0.f, 0.f, 1.f };

	//Required for Spotlight
	GLfloat CutOff = 45.0f;
	GLfloat* Direction = new GLfloat[3]{ 0.f, 0.f, 0.f };

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

	void SetLightType(Light light, LightType new_type);
};

#endif
