#include "Globals.h"
#include "Engine.h"
#include "ModuleLighting.h"
#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleLighting::ModuleLighting()
{
}

ModuleLighting::~ModuleLighting()
{
}

bool ModuleLighting::Init()
{
	return true;
}

bool ModuleLighting::Start()
{
	glEnable(GL_LIGHTING);

	//Add Default illumination;
	Lights[0].Number = GL_LIGHT0;
	Lights[1].Number = GL_LIGHT1;
	Lights[2].Number = GL_LIGHT2;
	Lights[3].Number = GL_LIGHT3;
	Lights[4].Number = GL_LIGHT4;
	Lights[5].Number = GL_LIGHT5;
	Lights[6].Number = GL_LIGHT6;
	Lights[7].Number = GL_LIGHT7;

	//enable 0 (combined light)
	Lights[0].IsEnabled = true;

	//Ambient light
	Ambient.Type = L_AMBIENT;
	Ambient.IsEnabled = false;

	return true;
}

update_status ModuleLighting::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleLighting::Update()
{
	if (Ambient.IsEnabled)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ambient.Ambient);

	for (Light light : Lights)
	{
		if(light.IsEnabled)
		{
			glEnable(light.Number);
			if (light.Type == L_COMBINED)
			{
				glLightfv(light.Number, GL_AMBIENT, light.Ambient);
				glLightfv(light.Number, GL_DIFFUSE, light.Diffuse);
				glLightfv(light.Number, GL_SPECULAR, light.Specular);
			}
			else if (light.Type == L_POINT)
			{
				glLightfv(light.Number, GL_POSITION, light.Position);
			}
			else if (light.Type == L_DIRECTIONAL)
			{
				glLightfv(light.Number, GL_POSITION, light.Position);
			}
			else if (light.Type == L_SPOTLIGHT)
			{
				glLightfv(light.Number, GL_POSITION, light.Position);
				glLightf(light.Number, GL_SPOT_CUTOFF, light.CutOff);
				glLightfv(light.Number, GL_SPOT_DIRECTION, light.Direction);
			}
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleLighting::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleLighting::CleanUp()
{
	return true;
}
