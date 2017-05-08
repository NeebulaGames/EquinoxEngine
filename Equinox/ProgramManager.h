#ifndef __PROGRAM_MANAGER__H__
#define __PROGRAM_MANAGER__H__

#include "Module.h"
#include <map>

class ProgramManager : public Module
{
public:
	ProgramManager();
	~ProgramManager();

	bool Init() override;
	bool CleanUp() override;

	void Load(std::string name, char* filepath, GLenum shaderType);

private:
	std::map<std::string, GLuint> programs;
};

#endif

