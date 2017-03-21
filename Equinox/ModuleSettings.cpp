#include "ModuleSettings.h"
#include "parson.h"

ModuleSettings::ModuleSettings(bool start_enabled) : Module(start_enabled)
{
}

ModuleSettings::~ModuleSettings()
{
}

bool ModuleSettings::Init()
{
	rootValue = json_parse_file(inputFile);
	settings = json_value_get_object(rootValue);

	if(settings != nullptr)
	{
		if (json_object_has_value(settings, "maxFps"))
			MaxFps = static_cast<int>(json_object_get_number(settings, "maxFps"));
		else
			MaxFps = 60;
		return true;
	}

	return false;
}

bool ModuleSettings::CleanUp()
{
	json_value_free(rootValue);
	return true;
}
