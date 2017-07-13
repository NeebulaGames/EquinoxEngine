#include "ModuleSceneManager.h"
#include "Scene.h"
#include <cassert>
#include <Brofiler.h>


ModuleSceneManager::ModuleSceneManager()
{
}


ModuleSceneManager::~ModuleSceneManager()
{
}

void ModuleSceneManager::SetInitialScene(Scene* initial)
{
	assert(_currentScene == nullptr);
	_currentScene = initial;
}

void ModuleSceneManager::SetScene(Scene* scene)
{
	assert(scene != nullptr);

	if (_currentScene) 
	{
		_currentScene->Disable();
		RELEASE(_currentScene);
	}

	_currentScene = scene;
	_currentScene->Enable();
}

Scene* ModuleSceneManager::CurrentScene() const
{
	return _currentScene;
}

void ModuleSceneManager::SetEntityFactory(BaseFactory<Entity>* factory)
{
	entity_factory = factory;
}

BaseFactory<Entity>* ModuleSceneManager::GetEntityFactory() const
{
	return entity_factory;
}

bool ModuleSceneManager::Start()
{
	return _currentScene ? (_currentScene->IsEnabled() ? _currentScene->Start() : _currentScene->Enable()) : true;
}

update_status ModuleSceneManager::PreUpdate(float DeltaTime)
{ BROFILER_CATEGORY("PreUpdateSceneManager", Profiler::Color::Brown)
	return _currentScene ? _currentScene->PreUpdate(0) : UPDATE_CONTINUE;
}

update_status ModuleSceneManager::Update(float DeltaTime)
{ BROFILER_CATEGORY("UpdateSceneManager", Profiler::Color::BurlyWood)
	return _currentScene ? _currentScene->Update(0) : UPDATE_CONTINUE;
}

update_status ModuleSceneManager::PostUpdate(float DeltaTime)
{ BROFILER_CATEGORY("PostUpdateSceneManager", Profiler::Color::CadetBlue)
	return _currentScene ? _currentScene->PostUpdate(0) : UPDATE_CONTINUE;
}

bool ModuleSceneManager::CleanUp()
{
	bool res = _currentScene ? _currentScene->CleanUp() : true;
	RELEASE(_currentScene);
	return res;
}
