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
	for (int i = 0; i < 8; ++i)
	{
		Lights[i] = new Light;
	}

	//Add Default illumination;
	Lights[0]->Number = GL_LIGHT0;
	Lights[1]->Number = GL_LIGHT1;
	Lights[2]->Number = GL_LIGHT2;
	Lights[3]->Number = GL_LIGHT3;
	Lights[4]->Number = GL_LIGHT4;
	Lights[5]->Number = GL_LIGHT5;
	Lights[6]->Number = GL_LIGHT6;
	Lights[7]->Number = GL_LIGHT7;

	return true;
}

bool ModuleLighting::Start()
{
	//TODO: Do we really need the glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AmbientLight.Ambient); ???

	////enable 0 sample (combined light)
	//Lights[0]->IsEnabled = false;

	////////////////
	//Lights[1]->IsEnabled = true;
	//Lights[1]->Type = L_POINT;
	//Lights[1]->Position[0] = 0.f;
	//Lights[1]->Position[1] = 20.f;
	//Lights[1]->Position[2] = 0.f;
	//Lights[1]->Position[3] = 1.f;

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

	for (Light* light : Lights)
	{
		if(light->IsEnabled)
		{
			glEnable(light->Number);

			/*if (light->Type == L_DEFAULT)
			{
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light->Ambient);
			}*/
			
			glLightfv(light->Number, GL_AMBIENT, light->Ambient);
			glLightfv(light->Number, GL_DIFFUSE, light->Diffuse);
			glLightfv(light->Number, GL_SPECULAR, light->Specular);

			glLightfv(light->Number, GL_POSITION, light->Position);

			if (light->Type == L_SPOTLIGHT)
			{
				glLightf(light->Number, GL_SPOT_CUTOFF, light->CutOff);
				glLightfv(light->Number, GL_SPOT_DIRECTION, light->Direction);
			}
		}
		else
		{
			glDisable(light->Number);
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
	for (Light* light : Lights)
	{
		RELEASE_ARRAY(light->Ambient);
		RELEASE_ARRAY(light->Diffuse);
		RELEASE_ARRAY(light->Specular);
		RELEASE_ARRAY(light->Position);
		RELEASE_ARRAY(light->Direction);

		RELEASE(light);
	}

	return true;
}

void ModuleLighting::SetLightType(Light* light, LightType new_type)
{
	light->Type = new_type;

	//default point
	GLfloat default_point[4] = { 0.f, 0.f, 0.f, 1.f };
	memcpy(light->Position, default_point, sizeof(GLfloat) * 4);
	
	//default spotlight values
	light->CutOff = 45.0f;
	GLfloat default_direction[3] = { 0.f, 0.f, 0.f };
	memcpy(light->Direction, default_direction, sizeof(GLfloat) * 3);

	if (new_type == L_DIRECTIONAL)
	{
		light->Position[2] = -1.f;
		light->Position[3] = 0.f;
	}
	else
	{
		light->Position[2] = 0.f;
		light->Position[3] = 1.f;
	}
		
}

LightType ModuleLighting::GetTypeByLabel(int label)
{
	switch (label)
	{
	case 0: return L_POINT;
	case 1: return L_DIRECTIONAL;
	case 2: return L_SPOTLIGHT;
	case 3: return L_DEFAULT;
	default: return L_DEFAULT;
	}
}

int ModuleLighting::GetLabelByType(LightType type)
{
	switch (type)
	{
	case L_POINT: return 0;
	case L_DIRECTIONAL: return 1;
	case L_SPOTLIGHT: return 2;
	case L_DEFAULT: return 3;
	default: return -1;
	}
}
