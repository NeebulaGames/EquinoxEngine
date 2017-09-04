#include "Level.h"
#include "ParticleEmitter.h"
#include "ModuleAnimation.h"
#include "ModuleTextures.h"
#include "TransformComponent.h"
#include "ModuleLevelManager.h"

ModuleLevelManager::ModuleLevelManager()
{
}


ModuleLevelManager::~ModuleLevelManager()
{
}

bool ModuleLevelManager::Init()
{
	_currentLevel = new Level;

	return true;
}

bool ModuleLevelManager::Start()
{
	_currentLevel->Load("Models/street/", "Street.obj");
	App->animator->Load("Idle", "Models/ArmyPilot/Animations/ArmyPilot_Idle.fbx");

	////////////
	GameObject* goPS = new GameObject;
	TransformComponent* transform = new TransformComponent;
	ParticleEmitter* peComponent = new ParticleEmitter(200, float2(50.f, 50.f), 20.f, 1.2f, 15.f);
	unsigned rainTex = App->textures->Load("Models/rainSprite.tga");
	//unsigned snowTex = App->textures->Load("Models/simpleflake.tga");
	peComponent->SetTexture(rainTex);
	goPS->Name = "ParticleSystem";
	goPS->AddComponent(transform);
	goPS->AddComponent(peComponent);

	_currentLevel->AddToScene(goPS);

	return true;
}

update_status ModuleLevelManager::PreUpdate(float DeltaTime)
{
	return UPDATE_CONTINUE;
}

update_status ModuleLevelManager::Update(float DeltaTime)
{
	_currentLevel->Update(DeltaTime);
	_currentLevel->DrawUI();
	return UPDATE_CONTINUE;
}

update_status ModuleLevelManager::PostUpdate(float DeltaTime)
{
	return UPDATE_CONTINUE;
}

bool ModuleLevelManager::CleanUp()
{
	_currentLevel->CleanUp();
	RELEASE(_currentLevel);

	return true;
}

Level* ModuleLevelManager::GetCurrentLevel() const
{
	return _currentLevel;
}
