#include "Globals.h"
#include "Engine.h"
#include "ModuleLighting.h"
#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <MathGeoLib/include/Math/float4.h>

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
	//Add Default illumination;
	Lights[0].Number = GL_LIGHT0;
	Lights[1].Number = GL_LIGHT1;
	Lights[2].Number = GL_LIGHT2;
	Lights[3].Number = GL_LIGHT3;
	Lights[4].Number = GL_LIGHT4;
	Lights[5].Number = GL_LIGHT5;
	Lights[6].Number = GL_LIGHT6;
	Lights[7].Number = GL_LIGHT7;

	//enable 0 sample (combined light)
	Lights[0].IsEnabled = false;

	//////////////
	Lights[1].IsEnabled = true;
	Lights[1].Type = L_POINT;
	Lights[1].Position[0] = 0.f;
	Lights[1].Position[1] = 20.f;
	Lights[1].Position[2] = 0.f;
	Lights[1].Position[3] = 1.f;

	//Ambient light
	AmbientLight.IsEnabled = false;

	return true;
}

update_status ModuleLighting::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleLighting::Update()
{
	//TODO: it will be nice to add something similar to a gizmod to "see" the light source object in the editor.
	glEnable(GL_LIGHTING);
	if (AmbientLight.IsEnabled)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AmbientLight.Ambient);

	for (Light light : Lights)
	{
		if(light.IsEnabled)
		{
			glEnable(light.Number);

			glLightfv(light.Number, GL_AMBIENT, light.Ambient);
			glLightfv(light.Number, GL_DIFFUSE, light.Diffuse);
			glLightfv(light.Number, GL_SPECULAR, light.Specular);
			
			if (light.Type == L_POINT)
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
		if (!light.IsEnabled)
		{
			glDisable(light.Number);
		}
	}
	glDisable(GL_LIGHTING);

	return UPDATE_CONTINUE;
}

update_status ModuleLighting::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleLighting::CleanUp()
{
	for (Light light : Lights)
	{
		RELEASE_ARRAY(light.Ambient);
		RELEASE_ARRAY(light.Diffuse);
		RELEASE_ARRAY(light.Specular);
		RELEASE_ARRAY(light.Position);
		RELEASE_ARRAY(light.Direction);
	}
	RELEASE_ARRAY(AmbientLight.Ambient);
	RELEASE_ARRAY(AmbientLight.Diffuse);
	RELEASE_ARRAY(AmbientLight.Specular);
	RELEASE_ARRAY(AmbientLight.Position);
	RELEASE_ARRAY(AmbientLight.Direction);
	return true;
}

void ModuleLighting::SetLightType(Light light, LightType new_type)
{
	light.Type = new_type;

	//default point
	GLfloat default_point[4] = { 0.f, 0.f, 0.f, 0.f };
	memcpy(light.Position, default_point, sizeof(GLfloat) * 4);
	
	//default spotlight values
	light.CutOff = 45.0f;
	GLfloat default_direction[3] = { 0.f, 0.f, 0.f };
	memcpy(light.Direction, default_direction, sizeof(GLfloat) * 3);

	if (new_type == L_DIRECTIONAL)
		light.Position[3] = 0.f;
	else
		light.Position[3] = 1.f;
}	
