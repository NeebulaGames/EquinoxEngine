#pragma once
#include "Module.h"
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
};

