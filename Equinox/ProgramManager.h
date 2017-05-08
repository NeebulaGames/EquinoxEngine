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

	void Load(const std::string &name, const char* filepath, const GLenum shaderType);

	GLuint GetProgramByName(const std::string &name) const;
	void UseProgram(const std::string &name) const;

private:
	std::map<std::string, GLuint> programs;

	void logShaderCompiler(const GLuint shader) const;
	void logProgramLinker(const GLuint program) const;
};

#endif

