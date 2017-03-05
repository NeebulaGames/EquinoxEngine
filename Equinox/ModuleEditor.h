#pragma once
#include "Module.h"
#include <list>

class ModuleEditor :
	public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Start() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;

private:
	bool _wireframe = false;
	std::list<float> _fpsValues;

	static float ListGetter(void* data, int id);
};

