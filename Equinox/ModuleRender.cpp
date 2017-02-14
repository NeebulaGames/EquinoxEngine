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
#include "CoordinateArrows.h"
#include "IL/ilut_config.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "IL/ilu.h"

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
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		int w, h;
		SDL_GetWindowSize(App->window->window, &w, &h);
		App->editorCamera->SetAspectRatio(float(w) / float(h));

		GLubyte checkImage[CHECKERS_WIDTH][CHECKERS_HEIGHT][4];

		//Check Image
		for (int i = 0; i < CHECKERS_WIDTH; i++) {
			for (int j = 0; j < CHECKERS_HEIGHT; j++) {
				int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
				checkImage[i][j][0] = GLubyte(c);
				checkImage[i][j][1] = GLubyte(c);
				checkImage[i][j][2] = GLubyte(c);
				checkImage[i][j][3] = GLubyte(255);
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &ImageName);
		glBindTexture(GL_TEXTURE_2D, ImageName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
			0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

		// Activate DevIL (Move to TextureManager)
		ilInit();
		iluInit();
		ilutInit();

		// Enable OpenGL access to DevIL
		ilutRenderer(ILUT_OPENGL);
		ilutEnable(ILUT_OPENGL_CONV);

		lenaImage = ilutGLLoadImage("Lenna.png");

		Quat rotation_plane = Quat::FromEulerXYZ(DEG2RAD(0.f), DEG2RAD(0.f), DEG2RAD(0.f));
		Quat rotation_cube = Quat::FromEulerXYZ(DEG2RAD(0.f), DEG2RAD(0.f), DEG2RAD(0.f));
		Quat rotation_sphere = Quat::FromEulerXYZ(DEG2RAD(0.f), DEG2RAD(0.f), DEG2RAD(0.f));
		Quat rotation_cylinder = Quat::FromEulerXYZ(DEG2RAD(0.f), DEG2RAD(0.f), DEG2RAD(0.f));

		objects.push_back(new Cube(float3(0.f, 0.f, -5.f), rotation_cube, ImageName));
		objects.push_back(new ::Plane(float3(0, 0.f, -5.f), rotation_plane, 60));
		objects.push_back(new Cube(float3(5.f, 0.f, -5.f), rotation_cube, lenaImage));
		objects.push_back(new ::Cylinder(float3(-2.f, 3.f, -5.f), rotation_cylinder, float3(0.f, 0.f, 25.f), 0.3f, 1.5));
		objects.push_back(new ::Sphere(float3(2, 2, -5.f), rotation_sphere, float3(25.f, 21.75f, 0), 1, 12, 24));
		Model* batman = new Model();
		batman->Load("Models/Batman/batman.obj");
		batman->Position.x = 10;
		objects.push_back(batman);
		objects.push_back(new CoordinateArrows());
	}
	return ret;
}

update_status ModuleRender::PreUpdate()
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
update_status ModuleRender::Update()
{
	bool ret = true;

	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	GLfloat light_position[] = { 0.25f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	for (std::list<Primitive*>::iterator it = objects.begin(); it != objects.end(); ++it)
		(*it)->Draw();

	return ret ? UPDATE_CONTINUE : UPDATE_ERROR;
}

update_status ModuleRender::PostUpdate()
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

