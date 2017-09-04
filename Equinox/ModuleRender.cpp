#include "Model.h"
#include "Globals.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include <windows.h>
#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Plane.h"
#include "ModuleEditorCamera.h"
#include "ModuleAnimation.h"
#include "CoordinateArrows.h"
#include "IL/ilut_config.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "IL/ilu.h"
#include "ModuleTextures.h"
#include "Level.h"
#include "ParticleEmitter.h"
#include "TransformComponent.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	return true;
}

bool ModuleRender::Start()
{
	LOG("Creating Renderer context");
	bool ret = true;
	context = SDL_GL_CreateContext(App->window->window);

	if (context == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		GLenum err = glewInit();

		if (err != GLEW_OK)
		{
			LOG("Error initialising GLEW: %s", glewGetErrorString(err));
			return false;
		}

		LOG("Using Glew %s", glewGetString(GLEW_VERSION));
		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0, 0, 0, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		int w, h;
		SDL_GetWindowSize(App->window->window, &w, &h);
		App->editorCamera->SetAspectRatio(float(w) / float(h));

		Quat rotation_plane = Quat::FromEulerXYZ(DEG2RAD(0.f), DEG2RAD(0.f), DEG2RAD(0.f));
		objects.push_back(new ::Plane(float3(0, 0.f, -5.f), rotation_plane, 60));

		objects.push_back(new CoordinateArrows());
	}
	return ret;
}

update_status ModuleRender::PreUpdate(float DeltaTime)
{	
	ModuleEditorCamera* camera = App->editorCamera;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(camera->GetProjectionMatrix());

	if (App->input->GetWindowEvent(WE_RESIZE))
	{
		int w, h;
		SDL_GetWindowSize(App->window->window, &w, &h);
		camera->SetAspectRatio(float(w) / float(h));
		glViewport(0, 0, w, h);
	}

	glClearColor(0, 0, 0, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(camera->GetViewMatrix());

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update(float DeltaTime)
{
	bool ret = true;

	for (std::list<Primitive*>::iterator it = objects.begin(); it != objects.end(); ++it)
		(*it)->Draw();

	return ret ? UPDATE_CONTINUE : UPDATE_ERROR;
}

update_status ModuleRender::PostUpdate(float DeltaTime)
{
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if (context != nullptr)
	{
		SDL_GL_DeleteContext(context);
	}

	for (std::list<Primitive*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	return true;
}

