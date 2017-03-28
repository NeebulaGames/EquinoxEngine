#include "Engine.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneManager.h"
#include "ModuleCollision.h"
#include "ModuleTimer.h"
#include "ComplexTimer.h"
#include "ModuleEditorCamera.h"
#include "ModuleEditor.h"
#include "ModuleLighting.h"
#include "ModuleSettings.h"
#include "ModuleAnimation.h"
#include "ModuleStats.h"

using namespace std;

Engine* App;

Engine::Engine()
{
	state = State::CREATION;

	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(editorCamera = new ModuleEditorCamera());

	modules.push_back(editor = new ModuleEditor());
	modules.push_back(animator = new ModuleAnimation());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(lighting = new ModuleLighting());
	modules.push_back(audio = new ModuleAudio());
	modules.push_back(settings = new ModuleSettings());
	modules.push_back(stats = new ModuleStats);

	// Game Modules
	modules.push_back(scene_manager = new ModuleSceneManager());

	// Modules to draw on top of game logic
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(timer = new ModuleTimer());

	DeltaTime = 0.f;
	App = this;
}

Engine::~Engine()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

int Engine::Loop()
{
	int ret = EXIT_FAILURE;
	while (state != EXIT)
	{
		switch (state)
		{
		case CREATION:
			state = START;
			break;
		case START:

			LOG("Engine Init --------------");
			if (App->Init() == false)
			{
				LOG("Engine Init exits with error -----");
				state = EXIT;
			}
			else
			{
				state = UPDATE;
				LOG("Engine Update --------------");
			}

			break;

		case UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Engine Update exits with error -----");
				state = EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = FINISH;
		}
			break;

		case FINISH:

			LOG("Engine CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Engine CleanUp exits with error -----");
			}
			else
				ret = EXIT_SUCCESS;

			state = EXIT;

			break;
		}
	}

	return ret;
}

bool Engine::Init()
{
	stats->_total_complex_time.Start();
	stats->_total_simple_time.Start();
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Start the first scene --

	return ret;
}

update_status Engine::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate(DeltaTime);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update(DeltaTime);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate(DeltaTime);

	++stats->_total_frames;

	float currentFrameTime = float(stats->_total_simple_time.Read() / 1E3);
	DeltaTime = float(currentFrameTime - _timeFromLastFrame);
	
	stats->_current_fps = 1 / DeltaTime;

	stats->_current_avg = stats->_current_avg ? (stats->_current_avg + stats->_current_fps) / 2 : stats->_current_fps;

	if (stats->_current_fps >= settings->MaxFps) {
		double aSecond = 1E3;
		Uint32 timeToDelay = (aSecond / settings->MaxFps) - DeltaTime;
		SDL_Delay(timeToDelay);
	}

	_timeFromLastFrame = currentFrameTime;

	return ret;
}

bool Engine::CleanUp()
{
	bool ret = true;

	LOG("Total Time: %f microseconds", stats->_total_complex_time.Stop());
	LOG("Total Time: %i miliseconds", stats->_total_simple_time.Stop());
	LOG("Total Frames: %f", stats->_total_frames);
	LOG("Average FPS: %f", stats->_current_avg);

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();
	return ret;
}

