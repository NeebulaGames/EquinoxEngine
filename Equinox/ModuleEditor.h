#pragma once
#include "Module.h"
#include <list>
#include "GameObject.h"

class ModuleEditor :
	public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Start() override;
	update_status PreUpdate(float DeltaTime) override;
	update_status Update(float DeltaTime) override;
	update_status PostUpdate(float DeltaTime) override;
	bool CleanUp() override;

	GameObject* SelectedGameObject = nullptr;

	bool DrawHierachy = false;
	bool DrawQuadtree = false;

private:
	bool _wireframe = false;
	std::list<float> _fpsValues;

	static float ListGetter(void* data, int id);
};

