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

class ModuleStats;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleCollision;
class ModuleSceneManager;
class ModuleTimer;
class ModuleEditorCamera;
class ModuleEditor;
class ModuleSettings;
class ModuleLighting;
class ModuleAnimation;

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
	ModuleTimer* timer;
	ModuleEditorCamera* editorCamera;
	ModuleEditor* editor;
	ModuleSettings* settings;
	ModuleLighting* lighting;
	ModuleAnimation* animator;
	ModuleStats* stats;

	// Game modules ---
	ModuleSceneManager* scene_manager;

	float DeltaTime;

private:
	State state;

	std::list<Module*> modules;

	float _timeFromLastFrame = 0;
};

extern Engine* App;

#endif // __APPLICATION_CPP__