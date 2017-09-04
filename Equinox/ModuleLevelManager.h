#pragma once
#include "Module.h"

class Level;

class ModuleLevelManager :
	public Module
{
public:
	ModuleLevelManager();
	~ModuleLevelManager();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float DeltaTime) override;
	update_status Update(float DeltaTime) override;
	update_status PostUpdate(float DeltaTime) override;
	bool CleanUp() override;

	Level* GetCurrentLevel() const;

private:

	Level* _currentLevel = nullptr;
};

