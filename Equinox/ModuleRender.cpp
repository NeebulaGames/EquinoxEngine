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
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	context = SDL_GL_CreateContext(App->window->window);
	
	if(context == nullptr)
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

		//this must be on Start()
		int w, h;
		SDL_GetWindowSize(App->window->window, &w, &h);
		App->editorCamera->SetAspectRatio(float(w) / float(h));

		GLubyte checkImage[CHECKERS_WIDTH][CHECKERS_HEIGHT][4];

		//Check Image
		for (int i = 0; i < CHECKERS_WIDTH; i++) {
			for (int j = 0; j < CHECKERS_HEIGHT; j++) {
				int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
				checkImage[i][j][0] = (GLubyte)c;
				checkImage[i][j][1] = (GLubyte)c;
				checkImage[i][j][2] = (GLubyte)c;
				checkImage[i][j][3] = (GLubyte)255;
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

		//End Start()
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

	//Color c = cam->background;
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

	Quat rotation_plane = Quat::FromEulerXYZ(DEG2RAD(0.f), DEG2RAD(0.f), DEG2RAD(0.f));
	Quat rotation_cube = Quat::FromEulerXYZ(DEG2RAD(0.f), DEG2RAD(0.f), DEG2RAD(0.f));
	Quat rotation_sphere = Quat::FromEulerXYZ(DEG2RAD(0.f), DEG2RAD(0.f), DEG2RAD(0.f));
	Quat rotation_cylinder = Quat::FromEulerXYZ(DEG2RAD(0.f), DEG2RAD(0.f), DEG2RAD(0.f));

	Cube cube(float3(0.f, 0.f, -5.f), rotation_cube, ImageName);
	::Plane plane(float3(0, 0.f, -5.f), rotation_plane, 60);
	Cube cube2(float3(5.f, 0.f, -5.f), rotation_cube, lenaImage);
	//::Sphere sphere(float3(2, 2, -5.f), rotation_sphere, float3(25.f, 21.75f, 0), 1, 12, 24);
	::Cylinder cylinder(float3(-2.f, 3.f, -5.f), rotation_cylinder, float3(0.f, 0.f, 25.f), 0.3f, 1.5);

	CoordinateArrows coordArrows;
		
	plane.Draw();
	cube.Draw();
	cube2.Draw();
	//sphere.Draw();
	cylinder.Draw();

	coordArrows.Draw();

	return ret ? UPDATE_CONTINUE : UPDATE_ERROR;
}

update_status ModuleRender::PostUpdate()
{
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	while (!_background.empty())
	{
		RenderData* data = _background.front();
		RELEASE(data->rect);
		RELEASE(data);
		_background.pop();
	}

	while (!_foreground.empty())
	{
		RenderData* data = _foreground.top().second;
		RELEASE(data->rect);
		RELEASE(data);
		_foreground.pop();
	}

	while (!_quads.empty())
	{
		QuadData* data = _quads.front();
		RELEASE(data->rect);
		RELEASE(data->color);
		RELEASE(data);
		_quads.pop();
	}

	while (!_ui.empty())
	{
		RenderData* data = _ui.front();
		RELEASE(data->rect);
		RELEASE(data);
		_ui.pop();
	}

	//Destroy window
	if (context != nullptr)
	{
		SDL_GL_DeleteContext(context);
	}

	return true;
}

// Blit to screen
bool ModuleRender::BlitBackground(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed)
{
	bool ret = true;
	SDL_Rect* rect = new SDL_Rect;
	rect->x = x * SCREEN_SIZE;
	rect->y = y * SCREEN_SIZE;

	if(section != NULL)
	{
		rect->w = section->w;
		rect->h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);
	}

	if (rect->w > _background_height)
		_background_height = rect->h;

	rect->w *= SCREEN_SIZE;
	rect->h *= SCREEN_SIZE;

	_background.push(new RenderData({ false, texture, section, rect }));

	return ret;
}

bool ModuleRender::AbsoluteBlit(SDL_Texture* texture, int x, int y, int z, SDL_Rect* section, float speed, bool flip)
{
	bool ret = true;
	SDL_Rect* rect = new SDL_Rect;
	rect->x = int(speed) + x * SCREEN_SIZE;
	y += int(ZTOY(z, RenderingAngle));
	rect->y = int(speed) + y * SCREEN_SIZE; // TODO: Add z to y

	if (section != NULL)
	{
		rect->w = section->w;
		rect->h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);
	}

	rect->w *= SCREEN_SIZE;
	rect->h *= SCREEN_SIZE;

	_foreground.emplace(z, new RenderData({ flip, texture, section, rect }));

	return ret;
}

bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, int z, SDL_Rect* section, float speed, bool flip)
{
	if (_background_height)
		y = _background_height - y;
	z *= -1;

	return AbsoluteBlit(texture, x, y, z, section, speed, flip);
}

bool ModuleRender::AbsoluteDrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_Color* color = new SDL_Color({ r,g,b,a });
	
	SDL_Rect* rec = new SDL_Rect(rect);
	if (use_camera)
	{
		rec->x = int(rect.x * SCREEN_SIZE);
		rec->y = int(rect.y * SCREEN_SIZE);
		rec->w *= SCREEN_SIZE;
		rec->h *= SCREEN_SIZE;
	}

	_quads.emplace(new QuadData({ rec, color }));

	return ret;
}

bool ModuleRender::AbsoluteDrawQuad(const iRectangle3& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	int y = rect.Position.y + int(ZTOY(rect.Position.z, RenderingAngle));
	return AbsoluteDrawQuad({ rect.Position.x, y, rect.w, rect.h }, r, g, b, a, use_camera);
}

bool ModuleRender::DrawQuad(const iRectangle3& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	iRectangle3 rec = rect;
	if (_background_height)
		rec.Position.y = _background_height - rect.Position.y;
	rec.Position.z *= -1;

	return AbsoluteDrawQuad(rec, r, g, b, a, use_camera);
}

bool ModuleRender::BlitUI(SDL_Texture* texture, int x, int y, SDL_Rect* section)
{
	bool ret = true;
	SDL_Rect* rect = new SDL_Rect;
	rect->x = int(x) * SCREEN_SIZE;
	rect->y = int(y) * SCREEN_SIZE;

	if (section != nullptr)
	{
		rect->w = section->w;
		rect->h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, nullptr, nullptr, &rect->w, &rect->h);
	}

	rect->w *= SCREEN_SIZE;
	rect->h *= SCREEN_SIZE;

	_ui.push(new RenderData({ false, texture, section, rect }));

	return ret;
}

bool ModuleRender::DirectBlit(SDL_Texture* texture, int x, int y, SDL_Rect* section)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = int(x) * SCREEN_SIZE;
	rect.y = int(y) * SCREEN_SIZE;

	if (section != nullptr)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if (SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

