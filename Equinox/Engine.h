#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "ComplexTimer.h"
#include "SimpleTimer.h"

#include "SDL/include/SDL.h"

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleCollision;
class ModuleParticles;
class ModuleSceneManager;
class ModuleTimer;
class ModuleEditorCamera;
class ModuleEditor;
class ModuleSettings;

// Game modules ---

class Engine
{
public:

	enum State
	{
		CREATION,
		START,
		UPDATE,
		FINISH,
		EXIT
	};

	Engine();
	~Engine();

	int Loop();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleCollision* collision;
	ModuleParticles* particles;
	ModuleTimer* timer;
	ModuleEditorCamera* editorCamera;
	ModuleEditor* editor;
	ModuleSettings* settings;

	// Game modules ---
	ModuleSceneManager* scene_manager;

	float DeltaTime;

private:
	State state;

	std::list<Module*> modules;

	Uint64 _total_frames;
	ComplexTimer _total_complex_time;
	SimpleTimer _total_simple_time;

	double _current_avg = 0;
	double _current_fps = 0;

	double _timeFromLastFrame = 0;
};

extern Engine* App;

#endif // __APPLICATION_CPP__