#ifndef __MODULESETTINGS_H__
#define __MODULESETTINGS_H__

#include "Module.h"
#include "parson.h"

class ModuleSettings : public Module
{
public:
	ModuleSettings(bool start_enabled = true);
	~ModuleSettings();

	bool Init() override;
	bool CleanUp() override;

	int MaxFps = 0;

private:

	JSON_Object* settings = nullptr;
	

	const char* inputFile = "engine.json";
};

#endif // __MODULESETTINGS_H__
